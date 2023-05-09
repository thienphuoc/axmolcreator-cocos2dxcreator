/******************************************************************************
 * Spine Runtimes License Agreement
 * Last updated May 1, 2019. Replaces all prior versions.
 *
 * Copyright (c) 2013-2019, Esoteric Software LLC
 *
 * Integration of the Spine Runtimes into software or otherwise creating
 * derivative works of the Spine Runtimes is permitted under the terms and
 * conditions of Section 2 of the Spine Editor License Agreement:
 * http://esotericsoftware.com/spine-editor-license
 *
 * Otherwise, it is permitted to integrate the Spine Runtimes into software
 * or otherwise create derivative works of the Spine Runtimes (collectively,
 * "Products"), provided that each user of the Products must obtain their own
 * Spine Editor license and redistribution of the Products in any form must
 * include this license and copyright notice.
 *
 * THIS SOFTWARE IS PROVIDED BY ESOTERIC SOFTWARE LLC "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL ESOTERIC SOFTWARE LLC BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, BUSINESS
 * INTERRUPTION, OR LOSS OF USE, DATA, OR PROFITS) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#ifdef SPINE_UE4
#include "SpinePluginPrivatePCH.h"
#endif

#include <ax_spine/Event.h>

#include <ax_spine/EventData.h>

ax_spine::Event::Event(float time, const ax_spine::EventData &data) :
		_data(data),
		_time(time),
		_intValue(0),
		_floatValue(0),
		_stringValue(),
		_volume(1),
		_balance(0) {
}

const ax_spine::EventData &ax_spine::Event::getData() {
	return _data;
}

float ax_spine::Event::getTime() {
	return _time;
}

int ax_spine::Event::getIntValue() {
	return _intValue;
}

void ax_spine::Event::setIntValue(int inValue) {
	_intValue = inValue;
}

float ax_spine::Event::getFloatValue() {
	return _floatValue;
}

void ax_spine::Event::setFloatValue(float inValue) {
	_floatValue = inValue;
}

const ax_spine::String &ax_spine::Event::getStringValue() {
	return _stringValue;
}

void ax_spine::Event::setStringValue(const ax_spine::String &inValue) {
	_stringValue = inValue;
}


float ax_spine::Event::getVolume() {
	return _volume;
}

void ax_spine::Event::setVolume(float inValue) {
	_volume = inValue;
}

float ax_spine::Event::getBalance() {
	return _balance;
}

void ax_spine::Event::setBalance(float inValue) {
	_balance = inValue;
}
