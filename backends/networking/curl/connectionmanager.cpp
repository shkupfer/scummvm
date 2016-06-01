/* ScummVM - Graphic Adventure Engine
*
* ScummVM is the legal property of its developers, whose names
* are too numerous to list here. Please refer to the COPYRIGHT
* file distributed with this source distribution.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
*/

#define FORBIDDEN_SYMBOL_ALLOW_ALL

#include "backends/networking/curl/connectionmanager.h"
#include "backends/networking/curl/networkreadstream.h"
#include "common/debug.h"
#include "common/system.h"
#include "common/timer.h"
#include <curl/curl.h>

namespace Common {

DECLARE_SINGLETON(Networking::ConnectionManager);

}

namespace Networking {

ConnectionManager::ConnectionManager(): _multi(0), _timerStarted(false) {
	curl_global_init(CURL_GLOBAL_ALL);
	_multi = curl_multi_init();
}

ConnectionManager::~ConnectionManager() {
	//terminate all requests
	_handleMutex.lock();
	for (Common::Array<RequestWithCallback>::iterator i = _requests.begin(); i != _requests.end(); ++i) {
		Request *request = i->request;
		RequestCallback callback = i->callback;
		if (request) request->finish();
		delete request;
		if (callback) (*callback)(request);
	}
	_requests.clear();
	_handleMutex.unlock();

	//cleanup
	curl_multi_cleanup(_multi);
	curl_global_cleanup();
}

void ConnectionManager::registerEasyHandle(CURL *easy) {
	curl_multi_add_handle(_multi, easy);
}

Request *ConnectionManager::addRequest(Request *request, RequestCallback callback) {
	_requests.push_back(RequestWithCallback(request, callback));
	if (!_timerStarted) startTimer();
	return request;
}

//private goes here:

void connectionsThread(void *ignored) {
	ConnMan.handle();
}

void ConnectionManager::startTimer(int interval) {
	Common::TimerManager *manager = g_system->getTimerManager();
	if (manager->installTimerProc(connectionsThread, interval, 0, "Networking::ConnectionManager's Timer")) {
		_timerStarted = true;
	} else {
		warning("Failed to install Networking::ConnectionManager's timer");
	}
}

void ConnectionManager::stopTimer() {
	debug("timer stopped");
	Common::TimerManager *manager = g_system->getTimerManager();
	manager->removeTimerProc(connectionsThread);
	_timerStarted = false;
}

void ConnectionManager::handle() {
	//lock mutex here (in case another handle() would be called before this one ends)
	_handleMutex.lock();
	interateRequests();
	processTransfers();
	_handleMutex.unlock();
}

void ConnectionManager::interateRequests() {
	//call handle() of all running requests (so they can do their work)
	debug("handling %d request(s)", _requests.size());	
	for (Common::Array<RequestWithCallback>::iterator i = _requests.begin(); i != _requests.end();) {
		Request *request = i->request;
		if (!request || request->state() == FINISHED) {
			delete (i->request);
			if (i->callback) (*i->callback)(i->request); //that's not a mistake (we're passing an address and that method knows there is no object anymore)
			_requests.erase(i);
			continue;
		}
		
		if (request) {
			if (request->state() == PROCESSING) request->handle();
			else if (request->state() == RETRY) request->handleRetry();
		}

		++i;		
	}
	if (_requests.empty()) stopTimer();
}

void ConnectionManager::processTransfers() {
	//check libcurl's transfers and notify requests of messages from queue (transfer completion or failure)
	int transfersRunning;
	curl_multi_perform(_multi, &transfersRunning);

	int messagesInQueue;
	CURLMsg *curlMsg;
	while ((curlMsg = curl_multi_info_read(_multi, &messagesInQueue))) {
		CURL *easyHandle = curlMsg->easy_handle;

		NetworkReadStream *stream;
		curl_easy_getinfo(easyHandle, CURLINFO_PRIVATE, &stream);
		if (stream) stream->finished();

		if (curlMsg->msg == CURLMSG_DONE) {
			debug("ConnectionManager: SUCCESS (%d - %s)", curlMsg->data.result, curl_easy_strerror(curlMsg->data.result));
		} else {
			debug("ConnectionManager: FAILURE (CURLMsg (%d))", curlMsg->msg);
		}

		curl_multi_remove_handle(_multi, easyHandle);
	}
}

} // End of namespace Cloud
