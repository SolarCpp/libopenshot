/**
 * @file
 * @brief Source file for EffectBase class
 * @author Jonathan Thomas <jonathan@openshot.org>
 *
 * @section LICENSE
 *
 * Copyright (c) 2008-2014 OpenShot Studios, LLC
 * <http://www.openshotstudios.com/>. This file is part of
 * OpenShot Library (libopenshot), an open-source project dedicated to
 * delivering high quality video editing and animation solutions to the
 * world. For more information visit <http://www.openshot.org/>.
 *
 * OpenShot Library (libopenshot) is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * OpenShot Library (libopenshot) is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with OpenShot Library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "../include/ClipBase.h"

using namespace openshot;

// Generate Json::JsonValue for this object
Json::Value ClipBase::JsonValue() {

	// Create root json object
	Json::Value root;
	root["id"] = Id();
	root["position"] = Position();
	root["layer"] = Layer();
	root["start"] = Start();
	root["end"] = End();
	root["duration"] = Duration();

	// return JsonValue
	return root;
}

// Load Json::JsonValue into this object
void ClipBase::SetJsonValue(Json::Value root) {

	// Set data from Json (if key is found)
	if (!root["id"].isNull())
		Id(root["id"].asString());
	if (!root["position"].isNull())
		Position(root["position"].asDouble());
	if (!root["layer"].isNull())
		Layer(root["layer"].asInt());
	if (!root["start"].isNull())
		Start(root["start"].asDouble());
	if (!root["end"].isNull())
		End(root["end"].asDouble());
}
