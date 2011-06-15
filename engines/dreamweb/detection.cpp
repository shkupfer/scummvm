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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL: https://svn.scummvm.org:4444/svn/dreamweb/detection.cpp $
 * $Id: detection.cpp 3 2010-09-16 19:32:18Z megath $
 *
 */

#include "base/plugins.h"

#include "common/algorithm.h"
#include "common/system.h"

#include "dreamweb/dreamweb.h"

#include "engines/advancedDetector.h"

namespace DreamWeb {

struct DreamWebGameDescription {
	ADGameDescription desc;
};

} // End of namespace DreamWeb

static const PlainGameDescriptor dreamWebGames[] = {
	{ "dreamweb", "DreamWeb" },
	{ 0, 0 }
};

#include "dreamweb/detection_tables.h"

class DreamWebMetaEngine : public AdvancedMetaEngine {
public:
	DreamWebMetaEngine():
	AdvancedMetaEngine(DreamWeb::gameDescriptions, 
	sizeof(DreamWeb::DreamWebGameDescription), dreamWebGames) {
		_singleid = "dreamweb";
		_guioptions = Common::GUIO_NOMIDI;
	}

	virtual const char *getName() const {
		return "DreamWeb engine";
	}

	virtual const char *getOriginalCopyright() const {
		return "DreamWeb (C) Creative Reality";
	}

	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const;
	virtual bool hasFeature(MetaEngineFeature f) const;
	virtual SaveStateList listSaves(const char *target) const;
	virtual int getMaximumSaveSlot() const;
	virtual void removeSaveState(const char *target, int slot) const;
};

bool DreamWebMetaEngine::hasFeature(MetaEngineFeature f) const {
	switch(f) {
	case kSupportsListSaves:
	case kSupportsLoadingDuringStartup:
	case kSupportsDeleteSave:
		return true;
	default:
		return false;
	}
}

bool DreamWeb::DreamWebEngine::hasFeature(EngineFeature f) const {
	return false;
}

bool DreamWebMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	const DreamWeb::DreamWebGameDescription *gd = (const DreamWeb::DreamWebGameDescription *)desc;
	if (gd) {
		*engine = new DreamWeb::DreamWebEngine(syst, gd);
	}
	return gd != 0;
}

SaveStateList DreamWebMetaEngine::listSaves(const char *target) const {
	Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
	Common::StringArray files = saveFileMan->listSavefiles("DREAMWEB.D??");
	Common::sort(files.begin(), files.end());

	SaveStateList saveList;
	for(uint i = 0; i < files.size(); ++i) {
		const Common::String &file = files[i];
		Common::InSaveFile *stream = saveFileMan->openForLoading(file);
		if (!stream)
			error("cannot open save file %s", file.c_str());
		char name[13] = {};
		stream->seek(0x61);
		stream->read(name, sizeof(name) - 1);
		delete stream;

		SaveStateDescriptor sd(i, name);
		saveList.push_back(sd);
	}

	return saveList;
}

int DreamWebMetaEngine::getMaximumSaveSlot() const { return 6; }

void DreamWebMetaEngine::removeSaveState(const char *target, int slot) const {
}

#if PLUGIN_ENABLED_DYNAMIC(DREAMWEB)
	REGISTER_PLUGIN_DYNAMIC(DREAMWEB, PLUGIN_TYPE_ENGINE, DreamWebMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(DREAMWEB, PLUGIN_TYPE_ENGINE, DreamWebMetaEngine);
#endif

namespace DreamWeb {

Common::Error DreamWebEngine::loadGameState(int slot) {
	return Common::kNoError;
}

Common::Error DreamWebEngine::saveGameState(int slot, const char *desc) {
	return Common::kNoError;
}

bool DreamWebEngine::canLoadGameStateCurrently() {
	return false;
}

bool DreamWebEngine::canSaveGameStateCurrently() {
	return false;
}

} // End of namespace DreamWeb
