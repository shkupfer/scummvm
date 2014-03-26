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
 */

#include "illusions/illusions.h"
#include "illusions/actor.h"
#include "illusions/camera.h"
#include "illusions/cursor.h"
#include "illusions/dictionary.h"
#include "illusions/input.h"
#include "illusions/screen.h"
#include "illusions/scriptman.h"
#include "illusions/scriptopcodes.h"
#include "illusions/sequenceopcodes.h"

namespace Illusions {

// DefaultSequences

uint32 DefaultSequences::use(uint32 sequenceId) {
	ItemsIterator it = Common::find_if(_items.begin(), _items.end(), DefaultSequenceEqual(sequenceId));
	return it != _items.end() ? (*it)._newSequenceId : sequenceId;
}

void DefaultSequences::set(uint32 sequenceId, uint32 newSequenceId) {
	ItemsIterator it = Common::find_if(_items.begin(), _items.end(), DefaultSequenceEqual(sequenceId));
	if (it == _items.end())
		_items.push_back(DefaultSequence(sequenceId, newSequenceId));
	else if (sequenceId == newSequenceId)
		_items.remove_at(it - _items.begin());
	else
		(*it)._newSequenceId = newSequenceId;
}

// Actor

Actor::Actor(IllusionsEngine *vm)
	: _vm(vm), _pauseCtr(0) {
	_pauseCtr = 0;
	_spriteFlags = 0;
	_drawFlags = 0;
	_flags = 0;
	_scale = 100;
	_frameIndex = 0;
	_newFrameIndex = 0;
	_surfInfo._pixelSize = 0;
	_surfInfo._dimensions._width = 0;
	_surfInfo._dimensions._height = 0;
	_surface = 0;
	_frames = 0;
	_scaleLayer = 0;
	_priorityLayer = 0;
	_position.x = 0;
	_position.y = 0;
	_position2.x = 0;
	_position2.y = 0;
	_facing = 64;
	_fontId = 0;
	_actorIndex = 0;
	_parentObjectId = 0;
	_linkIndex = 0;
	_linkIndex2 = 0;
	for (uint i = 0; i < kSubObjectsCount; ++i)
		_subobjects[i] = 0;
	_notifyThreadId1 = 0;
	_notifyThreadId2 = 0;
	_surfaceTextFlag = 0;
	_entryTblPtr = 0;
	_seqCodeIp = 0;
	_sequenceId = 0;
	_seqCodeValue1 = 0;
	_seqCodeValue2 = 600;
	_seqCodeValue3 = 0;
	
	_notifyId3C = 0;

	_pathCtrY = 0;
	
	_controlRoutine = 0;
	setControlRoutine(new Common::Functor2Mem<Control*, uint32, void, Controls>(_vm->_controls, &Controls::actorControlRoutine));

#if 0 // TODO
	_field2 = 0;
	_path40 = 0;
	_path4C = 0;
	_pathFlag50 = 0;
	_pathCtrX = 0;
	_pathInitialPosFlag = 1;
	_pathInitialPos.x = 0;
	_pathInitialPos.y = 0;
	_namedPointsCount = 0;
	_namedPoints = 0;
	_field164 = 0;
	_pathWalkRects = 0;
	_pathWalkPoints = 0;
	_pathNode = 0;
	_pathPoints = 0;
	_pathPointIndex = 0;
	_pathPointsCount = 0;
	_regionLayer = 0;
	_transitionRegionId = 0;
	_field18C = 0;
	_field190 = 0;
	_field192 = 0;
	_field198 = 0;
#endif

}

void Actor::pause() {
	++_pauseCtr;
}

void Actor::unpause() {
	--_pauseCtr;
}

void Actor::createSurface(SurfInfo &surfInfo) {
	_surface = _vm->_screen->allocSurface(surfInfo);
	if (_frameIndex) {
		if (_surfaceTextFlag) {
			/* TODO
			Font *font = _vm->findFont(_fontId);
			_surface->fillRect(Common::Rect(surfInfo._dimensions._width, surfInfo._dimensions._height), 0);
			gfx_sub_40CA70(_surface, font, _field18C, _surfInfo._dimensions, _field198);
			*/
			_flags |= 0x4000;
		}
		else {
			_flags |= 0x2000;
			_flags |= 0x4000;
		}
	}
}

void Actor::destroySurface() {
	if (_surface) {
		_surface->free();
		delete _surface;
		_surface = 0;
	}
}

void Actor::initSequenceStack() {
	_seqStackCount = 0;
}

void Actor::pushSequenceStack(int16 value) {
	_seqStack[_seqStackCount++] = value;
}

int16 Actor::popSequenceStack() {
	return _seqStack[--_seqStackCount];
}

void Actor::setControlRoutine(ActorControlRoutine *controlRoutine) {
	delete _controlRoutine;
	_controlRoutine = controlRoutine;
}

void Actor::runControlRoutine(Control *control, uint32 deltaTime) {
	if (_controlRoutine)
		(*_controlRoutine)(control, deltaTime);
}

// Control

Control::Control(IllusionsEngine *vm)
	: _vm(vm) {
	_flags = 0;
	_pauseCtr = 0;
	_priority = 0;
	_objectId = 0;
	_unkPt.x = 0;
	_unkPt.y = 0;
	_pt.x = 0;
	_pt.y = 0;
	_feetPt.x = 0;
	_feetPt.y = 0;
	_position.x = 0;
	_position.y = 0;
	_actorTypeId = 0;
	_actor = 0;
	// TODO _buf = 0;
	_tag = _vm->_scriptMan->_activeScenes.getCurrentScene();
}

Control::~Control() {
}

void Control::pause() {

	_vm->_dict->setObjectControl(_objectId, 0);

	if (_objectId == 0x40004)
		_vm->_cursor->setControl(0);

	if (_actor && !(_actor->_flags & 0x0200))
		_actor->destroySurface();

}

void Control::unpause() {

	_vm->_dict->setObjectControl(_objectId, this);

	if (_objectId == 0x40004)
		_vm->_cursor->setControl(this);
  
	if (_actor && !(_actor->_flags & 0x0200)) {
		SurfInfo surfInfo;
		ActorType *actorType = _vm->_dict->findActorType(_actorTypeId);
		if (actorType)
			surfInfo = actorType->_surfInfo;
		else
			surfInfo = _actor->_surfInfo;
		_actor->createSurface(surfInfo);
	}

}

void Control::appearActor() {
	if (_objectId == 0x40004) {
		_vm->_cursor->show();
	} else {
		if (_actor->_frameIndex || _actorTypeId == 0x50004)
			_actor->_flags |= 1;
		else
			_actor->_flags |= 0x1000;
		for (uint i = 0; i < kSubObjectsCount; ++i)
			if (_actor->_subobjects[i]) {
				Control *subControl = _vm->_dict->getObjectControl(_actor->_subobjects[i]);
				subControl->appearActor();
			}
	}
}

void Control::disappearActor() {
	if (_objectId == 0x40004) {
		_vm->_cursor->hide();
	} else {
		_actor->_flags &= ~1;
		_actor->_flags &= ~0x1000;
		for (uint i = 0; i < kSubObjectsCount; ++i)
			if (_actor->_subobjects[i]) {
				Control *subControl = _vm->_dict->getObjectControl(_actor->_subobjects[i]);
				subControl->disappearActor();
			}
	}
}

bool Control::isActorVisible() {
	return (_actor->_flags & 1) != 0;
}

void Control::activateObject() {
	_flags |= 1;
	if (_actor) {
		for (uint i = 0; i < kSubObjectsCount; ++i)
			if (_actor->_subobjects[i]) {
				Control *subControl = _vm->_dict->getObjectControl(_actor->_subobjects[i]);
				subControl->activateObject();
			}
	}
}

void Control::deactivateObject() {
	_flags &= ~1;
	if (_actor) {
		for (uint i = 0; i < kSubObjectsCount; ++i)
			if (_actor->_subobjects[i]) {
				Control *subControl = _vm->_dict->getObjectControl(_actor->_subobjects[i]);
				subControl->deactivateObject();
			}
	}
}

void Control::readPointsConfig(byte *pointsConfig) {
	_unkPt.x = READ_LE_UINT16(pointsConfig + 0);
	_unkPt.y = READ_LE_UINT16(pointsConfig + 2);
	pointsConfig += 4;
	_pt.x = READ_LE_UINT16(pointsConfig + 0);
	_pt.y = READ_LE_UINT16(pointsConfig + 2);
	pointsConfig += 4;
	_feetPt.x = READ_LE_UINT16(pointsConfig + 0);
	_feetPt.y = READ_LE_UINT16(pointsConfig + 2);
	pointsConfig += 4;
	_position.x = READ_LE_UINT16(pointsConfig + 0);
	_position.y = READ_LE_UINT16(pointsConfig + 2);
	pointsConfig += 4;
	for (uint i = 0; i < kSubObjectsCount; ++i) {
		_subobjectsPos[i].x = READ_LE_UINT16(pointsConfig + 0);
		_subobjectsPos[i].y = READ_LE_UINT16(pointsConfig + 2);
		pointsConfig += 4;
	}
}

void Control::setActorPosition(Common::Point position) {
	_actor->_position = position;
}

Common::Point Control::getActorPosition() {
	if (_actor)
		return _actor->_position;
	return _position;
}

void Control::setActorScale(int scale) {
	_actor->_scale = scale;
	for (uint i = 0; i < kSubObjectsCount; ++i)
		if (_actor->_subobjects[i]) {
			Control *subControl = _vm->_dict->getObjectControl(_actor->_subobjects[i]);
			subControl->activateObject();
		}
}

void Control::faceActor(uint facing) {
	_actor->_facing = facing;
	for (uint i = 0; i < kSubObjectsCount; ++i)
		if (_actor->_subobjects[i]) {
			Control *subControl = _vm->_dict->getObjectControl(_actor->_subobjects[i]);
			subControl->faceActor(facing);
		}
}

void Control::linkToObject(uint32 parentObjectId, uint32 linkedObjectValue) {
	_actor->_parentObjectId = parentObjectId;
	_actor->_linkIndex = linkedObjectValue;
}

void Control::unlinkObject() {
	_actor->_parentObjectId = 0;
	_actor->_linkIndex = 0;
}

void Control::clearNotifyThreadId1() {
	_actor->_notifyThreadId1 = 0;
}

void Control::clearNotifyThreadId2() {
	for (uint i = 0; i < kSubObjectsCount; ++i)
		if (_actor->_subobjects[i]) {
			Control *subControl = _vm->_dict->getObjectControl(_actor->_subobjects[i]);
			subControl->_actor->_flags &= ~0x80;
			subControl->_actor->_entryTblPtr = 0;
			subControl->_actor->_notifyThreadId2 = 0;
		}
	_actor->_flags &= ~0x80;
	_actor->_entryTblPtr = 0;
	_actor->_notifyThreadId2 = 0;
}

void Control::setPriority(int16 priority) {
	_priority = priority;
}

int Control::getPriority() {
	uint32 objectId;
	int16 positionY, priority, priority1;
	if (_actor) {
		if (_actor->_parentObjectId && (_actor->_flags & 0x40)) {
			uint32 parentObjectId = getSubActorParent();
			Control *parentControl = _vm->_dict->getObjectControl(parentObjectId);
			objectId = parentControl->_objectId;
			priority = parentControl->_priority;
			positionY = parentControl->_actor->_position.y;
			priority1 = _priority;
		} else {
			objectId = _objectId;
			positionY = _actor->_position.y;
			priority = _priority;
			priority1 = 50;
		}
	} else {
		positionY = _position.y;
		objectId = _objectId;
		priority = _priority;
		priority1 = 1;
	}

	priority -= 1;
	int p = 50 * priority1 / 100;
	if (p)
		--p;

	positionY = CLIP<int16>(positionY, -5000, 5000);

	return p + 50 * ((objectId & 0x3F) + ((10000 * priority + positionY + 5000) << 6));
}

Common::Point Control::calcPosition(Common::Point posDelta) {
	Common::Point pos;
	if (_actor->_parentObjectId) {
		int16 accuX = 0, accuY = 0;
		Actor *actor = _actor;
		while (actor->_parentObjectId) {
			Control *parentControl = _vm->_dict->getObjectControl(actor->_parentObjectId);
			accuX += parentControl->_subobjectsPos[actor->_linkIndex - 1].x;
			accuY += parentControl->_subobjectsPos[actor->_linkIndex - 1].y;
			actor = parentControl->_actor;
		}
		pos = actor->_position;
		pos.x += accuX * actor->_scale / 100;
		pos.y += accuY * actor->_scale / 100;
		_actor->_position = pos;
		if (!(_actor->_flags & 8)) {
			pos.x -= posDelta.x;
			pos.y -= posDelta.y;
		}
	} else {
		pos = _actor->_position;
		if (!(_actor->_flags & 8)) {
			pos.x -= posDelta.x;
			pos.y -= posDelta.y;
		}
	}
	return pos;
}

uint32 Control::getSubActorParent() {
	uint32 parentObjectId = _objectId;
	while (1) {
		Actor *actor = _vm->_dict->getObjectControl(parentObjectId)->_actor;
		if (actor->_parentObjectId && (actor->_flags & 0x40))
			parentObjectId = actor->_parentObjectId;
		else
			break;
	}
	return parentObjectId;
}

void Control::getCollisionRectAccurate(Common::Rect &collisionRect) {

	if (_actor && _actor->_frameIndex) {
		collisionRect = Common::Rect(-_position.x, -_position.y,
			-_position.x + _actor->_surfInfo._dimensions._width - 1,
			-_position.y + _actor->_surfInfo._dimensions._height - 1);
	} else {
		collisionRect = Common::Rect(_unkPt.x, _unkPt.y, _pt.x, _pt.y);
	}

	if (_actor) {
		if (_actor->_scale != 100) {
			// scaledValue = value * scale div 100
			collisionRect.left = collisionRect.left * _actor->_scale / 100;
			collisionRect.top = collisionRect.top * _actor->_scale / 100;
			collisionRect.right = collisionRect.right * _actor->_scale / 100;
			collisionRect.bottom = collisionRect.bottom * _actor->_scale / 100;
		}
		collisionRect.translate(_actor->_position.x, _actor->_position.y);
	}

	if (_flags & 8) {
		Common::Point screenOffs = _vm->_camera->getScreenOffset();
		collisionRect.translate(screenOffs.x, screenOffs.y);
	}

}

void Control::getCollisionRect(Common::Rect &collisionRect) {
	collisionRect = Common::Rect(_unkPt.x, _unkPt.y, _pt.x, _pt.y);
	if (_actor) {
		if (_actor->_scale != 100) {
			// scaledValue = value * scale div 100
			collisionRect.left = collisionRect.left * _actor->_scale / 100;
			collisionRect.top = collisionRect.top * _actor->_scale / 100;
			collisionRect.right = collisionRect.right * _actor->_scale / 100;
			collisionRect.bottom = collisionRect.bottom * _actor->_scale / 100;
		}
		collisionRect.translate(_actor->_position.x, _actor->_position.y);
	}
	if (_flags & 8) {
		Common::Point screenOffs = _vm->_camera->getScreenOffset();
		collisionRect.translate(screenOffs.x, screenOffs.y);
	}
}

void Control::setActorUsePan(int usePan) {
	if (usePan == 1)
		_flags &= ~8;
	else
		_flags |= 8;
}

void Control::setActorFrameIndex(int16 frameIndex) {
	if (frameIndex) {
		_actor->_frameIndex = frameIndex;
		const Frame &frame = (*_actor->_frames)[frameIndex - 1];
		_actor->_surfInfo = frame._surfInfo;
		readPointsConfig(frame._pointsConfig);
		_actor->_flags |= 0x2000;
		_actor->_flags |= 0x4000;
		_actor->_newFrameIndex = 0;
	}
}

void Control::stopActor() {
	_actor->_seqCodeIp = 0;
	/* TODO
	if (_actor->_pathNode) {
		if (_actor->_flags & 0x0400) {
			// TODO delete _actor->_pathNode;
			_actor->_flags &= ~0x0400;
		}
		_actor->_pathNode = 0;
		_actor->_pathPoints = 0;
		_actor->_pathPointsCount = 0;
		_actor->_pathPointIndex = 0;
		_actor->_path40 = 0;
	}
	*/
	_vm->notifyThreadId(_actor->_notifyThreadId1);
	_vm->notifyThreadId(_actor->_notifyId3C);
}

void Control::startSequenceActor(uint32 sequenceId, int value, uint32 notifyThreadId) {
	startSequenceActorIntern(sequenceId, value, 0, notifyThreadId);
}

void Control::stopSequenceActor() {
	if (_actor->_flags & 0x40) {
		stopActor();
		_actor->_frameIndex = 0;
		if ((_actor->_flags & 1) || (_actor->_flags & 0x1000)) {
			_actor->_flags &= ~1;
			_actor->_flags |= 0x1000;
		}
	}
	for (uint i = 0; i < kSubObjectsCount; ++i)
		if (_actor->_subobjects[i]) {
			Control *subControl = _vm->_dict->getObjectControl(_actor->_subobjects[i]);
			subControl->stopSequenceActor();
		}
}

void Control::startTalkActor(uint32 sequenceId, byte *entryTblPtr, uint32 threadId) {
	bool doSeq = true;
	if (_actor->_linkIndex2) {
		Control *subControl = _vm->_dict->getObjectControl(_actor->_subobjects[_actor->_linkIndex2 - 1]);
		if (subControl->_actor->_flags & 1) {
			/* TODO
			if (control->_actor->pathNode) {
				doSeq = false;
				subControl->_actor->notifyThreadId2 = threadId;
				subControl->_actor->entryTblPtr = entryTblPtr;
				subControl->_actor->flags |= 0x80;
				script_TalkThreads_sub_417FA0(subControl->_actor->_notifyThreadId2, 0);
			}
			*/
		}
	}
	if (doSeq)
		startSequenceActorIntern(sequenceId, 2, entryTblPtr, threadId);
}

void Control::sequenceActor() {

	if (_actor->_pauseCtr > 0)
		return;

	OpCall opCall;
  	bool sequenceFinished = false;

	opCall._result = 0;
	_actor->_seqCodeValue3 -= _actor->_seqCodeValue1;
	
	while (_actor->_seqCodeValue3 <= 0 && !sequenceFinished) {
		bool breakInner = false;
		while (!breakInner) {
			debug(1, "SEQ op: %08X", _actor->_seqCodeIp[0]);
			opCall._op = _actor->_seqCodeIp[0] & 0x7F;
			opCall._opSize = _actor->_seqCodeIp[1];
			opCall._code = _actor->_seqCodeIp + 2;
			opCall._deltaOfs = opCall._opSize;
			if (_actor->_seqCodeIp[0] & 0x80)
				breakInner = true;
			execSequenceOpcode(opCall);
			if (opCall._result == 1) {
				sequenceFinished = true;
				breakInner = true;
			} else if (opCall._result == 2) {
				breakInner = true;
			}
			_actor->_seqCodeIp += opCall._deltaOfs;
		}
		_actor->_seqCodeValue3 += _actor->_seqCodeValue2;
	}

	if (_actor->_newFrameIndex != 0) {
		debug(1, "New frame %d", _actor->_newFrameIndex);
		setActorFrameIndex(_actor->_newFrameIndex);
		if (!(_actor->_flags & 1) && (_actor->_flags & 0x1000) && (_objectId != 0x40004)) {
			appearActor();
			_actor->_flags &= ~0x1000;
		}
	}
	
	if (sequenceFinished) {
		debug(1, "Sequence has finished");
		_actor->_seqCodeIp = 0;
	}
	
}

void Control::setActorIndexTo1() {
	_actor->_actorIndex = 1;
}

void Control::setActorIndexTo2() {
	_actor->_actorIndex = 2;
}

void Control::startSequenceActorIntern(uint32 sequenceId, int value, byte *entryTblPtr, uint32 notifyThreadId) {

	stopActor();
	
	_actor->_flags &= ~0x80;
	_actor->_flags &= ~0x0400;
	_actor->_flags |= 0x0100;

	sequenceId = _actor->_defaultSequences.use(sequenceId);

	_actor->_sequenceId = sequenceId;
	_actor->_notifyThreadId1 = notifyThreadId;
	_actor->_notifyId3C = 0;
	_actor->_path40 = 0;
	
	Sequence *sequence = _vm->_dict->findSequence(sequenceId);
	//debug("sequence: %p", (void*)sequence);

	_actor->_seqCodeIp = sequence->_sequenceCode;
	_actor->_frames = _vm->_actorItems->findSequenceFrames(sequence);
	
	/*
	for (int i = 0; i < 64; ++i) {
		debugN("%02X ", sequence->_sequenceCode[i]);
	}
	debug(".");
	*/
	
	_actor->_seqCodeValue3 = 0;
	_actor->_seqCodeValue1 = 0;
	_actor->_seqCodeValue2 = value == 1 ? 350 : 600;
	_actor->initSequenceStack();
	stopSequenceActor();
	_actor->_linkIndex2 = 0;
	if (entryTblPtr) {
		_actor->_flags |= 0x80;
		_actor->_entryTblPtr = entryTblPtr;
		_actor->_notifyThreadId1 = 0;
		_actor->_notifyThreadId2 = notifyThreadId;
	}

	sequenceActor();
	
}

void Control::execSequenceOpcode(OpCall &opCall) {
	// TODO Clean this up
	_vm->_controls->_sequenceOpcodes->execOpcode(this, opCall);
}

// Controls

Controls::Controls(IllusionsEngine *vm)
	: _vm(vm) {
	_sequenceOpcodes = new SequenceOpcodes(_vm);
}

Controls::~Controls() {
	delete _sequenceOpcodes;
}

void Controls::placeBackgroundObject(BackgroundObject *backgroundObject) {
	Control *control = newControl();
	control->_objectId = backgroundObject->_objectId;
	control->_flags = backgroundObject->_flags;
	control->_priority = backgroundObject->_priority;
	control->readPointsConfig(backgroundObject->_pointsConfig);
	control->activateObject();
	_controls.push_back(control);
	_vm->_dict->setObjectControl(control->_objectId, control);
}

void Controls::placeActor(uint32 actorTypeId, Common::Point placePt, uint32 sequenceId, uint32 objectId, uint32 notifyThreadId) {
	Control *control = newControl();
	Actor *actor = newActor();

	ActorType *actorType = _vm->_dict->findActorType(actorTypeId);
	control->_objectId = objectId;
	control->_flags = actorType->_flags;
	control->_priority = actorType->_priority;
	control->readPointsConfig(actorType->_pointsConfig);
	control->_actorTypeId = actorTypeId;
	control->_actor = actor;
	if (actorTypeId == 0x50001 && objectId == 0x40004)
		actor->setControlRoutine(new Common::Functor2Mem<Control*, uint32, void, Cursor>(_vm->_cursor, &Cursor::cursorControlRoutine));
	if (actorType->_surfInfo._dimensions._width > 0 || actorType->_surfInfo._dimensions._height > 0) {
		actor->createSurface(actorType->_surfInfo);
	} else {
		actor->_flags |= 0x0200;
	}
	actor->_position = placePt;
	actor->_position2 = placePt;
	Common::Point currPan = _vm->_camera->getCurrentPan();
	// TODO if (!artcntrl_calcPointDirection(placePt, panPos, &actor->facing))
	actor->_facing = 64;
	actor->_scale = actorType->_scale;
	// TODO actor->_namedPointsCount = actorType->_namedPointsCount;
	// TODO actor->_namedPoints = actorType->_namedPoints;
	
	BackgroundResource *bgRes = _vm->_backgroundItems->getActiveBgResource();
	if (actorType->_pathWalkPointsIndex) {
		// TODO actor->_pathWalkPoints = bgRes->getPathWalkPoints(actorType->_pathWalkPointsIndex - 1);
		actor->_flags |= 0x02;
	}

	if (actorType->_scaleLayerIndex) {
		actor->_scaleLayer = bgRes->getScaleLayer(actorType->_scaleLayerIndex - 1);
		actor->_flags |= 0x04;
	}

	if (actorType->_pathWalkRectIndex) {
		// TODO actor->_pathWalkRects = bgRes->getPathWalkRects(actorType->_pathWalkRectIndex - 1);
		actor->_flags |= 0x10;
	}
	
	if (actorType->_priorityLayerIndex) {
		actor->_priorityLayer = bgRes->getPriorityLayer(actorType->_priorityLayerIndex - 1);
		actor->_flags |= 0x08;
	}
	
	if (actorType->_regionLayerIndex) {
		// TODO actor->_regionLayer = bgRes->getPriorityLayer(actorType->_regionLayerIndex - 1);
		actor->_flags |= 0x20;
	}
	
	actor->_pathCtrY = 140;
	
	_controls.push_back(control);
	_vm->_dict->setObjectControl(objectId, control);

	if (actorTypeId == 0x50001 && objectId == 0x40004)
		_vm->_cursor->place(control, sequenceId);

	control->_flags |= 0x01;
	actor->_flags |= 0x1000;

	control->startSequenceActor(sequenceId, 2, notifyThreadId);
}

void Controls::placeSequenceLessActor(uint32 objectId, Common::Point placePt, WidthHeight dimensions, int16 priority) {
	Control *control = newControl();
	Actor *actor = newActor();
	control->_flags = 0;
	control->_priority = priority;
	control->_objectId = objectId;
	control->_unkPt.x = 0;
	control->_unkPt.y = 0;
	control->_pt.y = dimensions._height - 1;
	control->_pt.x = dimensions._width - 1;
	control->_feetPt.x = dimensions._width / 2;
	control->_feetPt.y = dimensions._height / 2;
	control->_position.x = 0;
	control->_position.y = 0;
	control->_actorTypeId = 0x50004;
	control->_actor = actor;
	actor->setControlRoutine(0);
	actor->_surfInfo._pixelSize = dimensions._width * dimensions._height;
	actor->_surfInfo._dimensions = dimensions;
	actor->createSurface(actor->_surfInfo);
	actor->_position = placePt;
	actor->_position2 = placePt;
	actor->_facing = 64;
	actor->_scale = 100;
	// TODO actor->_namedPointsCount = 0;
	// TODO actor->_namedPoints = 0;
	actor->_pathCtrY = 140;

	_controls.push_back(control);
	_vm->_dict->setObjectControl(objectId, control);
	control->appearActor();
}

void Controls::placeActorLessObject(uint32 objectId, Common::Point feetPt, Common::Point pt, int16 priority, uint flags) {
	Control *control = newControl();
	control->_flags = flags;
	control->_unkPt = feetPt;
	control->_feetPt = feetPt;
	control->_priority = priority;
	control->_objectId = objectId;
	control->_pt = pt;
	control->_position.x = 0;
	control->_position.y = 0;
	control->_actorTypeId = 0;
	control->_actor = 0;
	_controls.push_back(control);
	_vm->_dict->setObjectControl(objectId, control);
}

void Controls::destroyControlsByTag(uint32 tag) {
	ItemsIterator it = _controls.begin();
	while (it != _controls.end()) {
		if ((*it)->_tag == tag) {
			destroyControl(*it);
			it = _controls.erase(it);
		} else
			++it;			
	}
}

void Controls::pauseControlsByTag(uint32 tag) {
	for (ItemsIterator it = _controls.begin(); it != _controls.end(); ++it) {
		Control *control = *it;
		if (control->_tag == tag) {
			++control->_pauseCtr;
			if (control->_pauseCtr == 1)
				control->pause();
		}
	}
}

void Controls::unpauseControlsByTag(uint32 tag) {
	for (ItemsIterator it = _controls.begin(); it != _controls.end(); ++it) {
		Control *control = *it;
		if (control->_tag == tag) {
			--control->_pauseCtr;
			if (control->_pauseCtr == 0)
				control->unpause();
		}
	}
}

bool Controls::getOverlappedObject(Control *control, Common::Point pt, Control **outOverlappedControl, int minPriority) {
	Control *foundControl = 0;
	int foundPriority = 0;
	// TODO minPriority = artcntrlGetPriorityFromBase(minPriority);

	for (ItemsIterator it = _controls.begin(); it != _controls.end(); ++it) {
		Control *testControl = *it;
		if (testControl != control && testControl->_pauseCtr == 0 &&
			(testControl->_flags & 1) && !(testControl->_flags & 0x10) &&
			(!testControl->_actor || (testControl->_actor->_flags & 1))) {
			Common::Rect collisionRect;
			testControl->getCollisionRect(collisionRect);
			//debug("collisionRect(%d, %d, %d, %d)", collisionRect.left, collisionRect.top, collisionRect.right, collisionRect.bottom);
			//debug("pt(%d, %d)", pt.x, pt.y);
			if (!collisionRect.isEmpty() && collisionRect.contains(pt)) {
				int testPriority = testControl->getPriority();
				//debug("testPriority: %d; minPriority: %d", testPriority, minPriority);
				if ((!foundControl || foundPriority < testPriority) &&
					testPriority >= minPriority) {
					//debug("overlapped() %08X; pauseCtr: %d; flags: %04X", testControl->_objectId, testControl->_pauseCtr, testControl->_flags);
					foundControl = testControl;
					foundPriority = testPriority;
				}
			}		
		}
	}

	if (foundControl) {
		if (foundControl->_actor && foundControl->_actor->_parentObjectId && (foundControl->_actor->_flags & 0x40)) {
			uint32 parentObjectId = foundControl->getSubActorParent();
			foundControl = _vm->_dict->getObjectControl(parentObjectId);
		}
		*outOverlappedControl = foundControl;
	}

	return foundControl != 0;
}

void Controls::actorControlRoutine(Control *control, uint32 deltaTime) {

	Actor *actor = control->_actor;

	if (actor->_pauseCtr > 0)
		return;

	if (false/*actor->_pathNode*/) {
		// TODO Update pathwalking
	} else {
		actor->_seqCodeValue1 = 100 * deltaTime;
	}

	if (actor->_flags & 4) {
		int scale = actor->_scaleLayer->getScale(actor->_position);
		control->setActorScale(scale);
	}

	if (actor->_flags & 8) {
		int16 priority = actor->_priorityLayer->getPriority(actor->_position);
		if (priority)
			control->setPriority(priority + 1);
	}

	if (actor->_flags & 0x20) {
		// TODO Update transition sequence (seems to be unused in BBDOU?)
	}

}

Actor *Controls::newActor() {
	return new Actor(_vm);
}

Control *Controls::newControl() {
	return new Control(_vm);
}

void Controls::destroyControl(Control *control) {

	if (control->_pauseCtr <= 0)
		_vm->_dict->setObjectControl(control->_objectId, 0);

	if (control->_objectId == 0x40004 && control->_pauseCtr <= 0)
		_vm->_cursor->setControl(0);
	
	if (control->_actor) {
		/* TODO
		if (actor->_pathNode && (actor->_flags & 0x400))
			delete actor->_pathNode;
		*/
		if (!(control->_actor->_flags & 0x200))
			control->_actor->destroySurface();
		/* TODO
		if (control->_actor->_field2)
			largeObj_sub_4061E0();
		*/
		delete control->_actor;
		control->_actor = 0;
	}
	/* TODO
	if (control->_buf)
		free(control->_buf);
	*/
	delete control;
}

} // End of namespace Illusions
