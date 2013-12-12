/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#include "Engine/savable.hpp"


/*----------------------------------------------
	Definitions
----------------------------------------------*/

const Ogre::String XML_FILE_DIR = "Content/Save/0000/";
const Ogre::String XML_TEMPLATE_DIR = "Content/Templates/";

const char* XML_FILE_VALUE_NAME = "value";
const char* XML_FILE_SAVE_NAME = "savefile";
const char* XML_FILE_TEMPLATE_NAME = "template";
const char* XML_FILE_HEADER = "xml version=\"1.0\" encoding=\"UTF-8\"";

const Quaternion LEFT =			Quaternion(Radian(Degree(-90).valueRadians()), Vector3(0,1,0));
const Quaternion RIGHT =		Quaternion(Radian(Degree(+90).valueRadians()), Vector3(0,1,0));
const Quaternion TOP =			Quaternion(Radian(Degree(-90).valueRadians()), Vector3(1,0,0));
const Quaternion BOTTOM =		Quaternion(Radian(Degree(+90).valueRadians()), Vector3(1,0,0));
const Quaternion FORWARD =		Quaternion(Radian(Degree(180).valueRadians()), Vector3(0,1,0));
const Quaternion BACK =			Quaternion(Radian(Degree(  0).valueRadians()), Vector3(0,1,0));


/*----------------------------------------------
	Public methods (save)
----------------------------------------------*/

void Savable::saveToFile()
{
	if (!isSavable())
	{
		return;
	}
	
	mIsSaving = true;
	mSaveFile = new tinyxml2::XMLDocument();

	if (mSaveFile)
	{
		mSaveFile->InsertEndChild(mSaveFile->NewDeclaration(XML_FILE_HEADER));
		mSaveData = mSaveFile->NewElement(XML_FILE_SAVE_NAME);

		if (mSaveData)
		{
			Ogre::String filePath = XML_FILE_DIR + getFileName();
			mSaveFile->InsertEndChild(mSaveData);
			save();

			bool res = (mSaveFile->SaveFile(filePath.c_str()) == tinyxml2::XML_NO_ERROR);
			assert(res && "Failed to save file");
		}
		else
		{
			assert(false && "Failed to insert save data");
		}
		delete mSaveFile;
	}
	else
	{	
		assert(false && "Failed to create a save file");
	}

	mIsSaving = false;
}


void Savable::loadFromFile()
{
	if (!mIsSaving)
	{
		mSaveFile = new tinyxml2::XMLDocument();
		if (mSaveFile->LoadFile((XML_FILE_DIR + getFileName()).c_str()) == tinyxml2::XML_NO_ERROR)
		{
			mSaveData = mSaveFile->FirstChildElement(XML_FILE_SAVE_NAME);
			load();
		}
		else
		{	
			assert(false && "Failed to load the save file");
		}
	}
	else
	{	
		assert(false && "Trying to load while saving");
	}
}


/*----------------------------------------------
	Protected methods (templates)
----------------------------------------------*/

void Savable::loadTemplate(String name)
{
	mIsSaving = false;
	mSaveFile = new tinyxml2::XMLDocument();

	if (mSaveFile->LoadFile((XML_TEMPLATE_DIR + name).c_str()) == tinyxml2::XML_NO_ERROR)
	{
		mSaveData = mSaveFile->FirstChildElement(XML_FILE_TEMPLATE_NAME);
	}
	else
	{	
		assert(false && "Failed to load template");
	}
}


void Savable::closeTemplate()
{
	delete mSaveFile;
}


void Savable::setTemplateGroup(String name)
{
	setSaveGroup(name);
}


/*----------------------------------------------
	Protected methods (saving)
----------------------------------------------*/

void Savable::setSaveGroup(String name)
{
	if (mIsSaving)
	{
		mSaveGroup = mSaveFile->NewElement(name.c_str());
		mSaveData->InsertEndChild(mSaveGroup);
	}
	else
	{
		mSaveGroup = mSaveData->FirstChildElement(name.c_str());
	}
}


bool Savable::isSavable()
{
	return false;
}


void Savable::saveValue(int value, String name)
{
	if (mIsSaving)
	{
		tinyxml2::XMLElement* attr = mSaveFile->NewElement(name.c_str());
		attr->SetAttribute(XML_FILE_VALUE_NAME, value);
		mSaveGroup->InsertEndChild(attr);
	}
}


/*----------------------------------------------
	Data save & load
----------------------------------------------*/

void Savable::saveValue(float value, String name)
{
	if (mIsSaving)
	{
		tinyxml2::XMLElement* attr = mSaveFile->NewElement(name.c_str());
		attr->SetAttribute(XML_FILE_VALUE_NAME, value);
		mSaveGroup->InsertEndChild(attr);
	}
}


void Savable::saveValue(String value, String name)
{
	if (mIsSaving)
	{
		tinyxml2::XMLElement* attr = mSaveFile->NewElement(name.c_str());
		attr->SetAttribute(XML_FILE_VALUE_NAME, value.c_str());
		mSaveGroup->InsertEndChild(attr);
	}
}


void Savable::saveValue(Vector3 value, String name)
{
	if (mIsSaving)
	{
		tinyxml2::XMLElement* attr = mSaveFile->NewElement(name.c_str());
		attr->SetAttribute(XML_FILE_VALUE_NAME, StringConverter::toString(value).c_str());
		mSaveGroup->InsertEndChild(attr);
	}
}


void Savable::saveValue(Quaternion value, String name)
{
	if (mIsSaving)
	{
		tinyxml2::XMLElement* attr = mSaveFile->NewElement(name.c_str());
		attr->SetAttribute(XML_FILE_VALUE_NAME, StringConverter::toString(value).c_str());
		mSaveGroup->InsertEndChild(attr);
	}
}


void Savable::saveValue(Ogre::ColourValue value, String name)
{
	if (mIsSaving)
	{
		tinyxml2::XMLElement* attr = mSaveFile->NewElement(name.c_str());
		attr->SetAttribute(XML_FILE_VALUE_NAME, StringConverter::toString(value).c_str());
		mSaveGroup->InsertEndChild(attr);
	}
}


int Savable::loadIntValue(String name)
{
	int value = 0;
	tinyxml2::XMLElement* attr = mSaveGroup->FirstChildElement(name.c_str());
	if (attr)
	{
		value = attr->IntAttribute(XML_FILE_VALUE_NAME);
	}
	return value;
}


float Savable::loadFloatValue(String name)
{
	float value = 0.0f;
	tinyxml2::XMLElement* attr = mSaveGroup->FirstChildElement(name.c_str());
	if (attr)
	{
		value = attr->FloatAttribute(XML_FILE_VALUE_NAME);
	}
	return value;
}


String Savable::loadStringValue(String name)
{
	String value;
	tinyxml2::XMLElement* attr = mSaveGroup->FirstChildElement(name.c_str());
	if (attr)
	{
		value = String(attr->Attribute(XML_FILE_VALUE_NAME));
	}
	return value;
}


Vector3 Savable::loadVectorValue(String name)
{
	Vector3 value;
	tinyxml2::XMLElement* attr = mSaveGroup->FirstChildElement(name.c_str());
	if (attr)
	{
		value = Ogre::StringConverter::parseVector3(attr->Attribute(XML_FILE_VALUE_NAME));
	}
	return value;
}


Quaternion Savable::loadQuaternionValue(String name)
{
	Quaternion value;
	tinyxml2::XMLElement* attr = mSaveGroup->FirstChildElement(name.c_str());
	if (attr)
	{
		value = Ogre::StringConverter::parseQuaternion(attr->Attribute(XML_FILE_VALUE_NAME));
	}
	return value;
}


Ogre::ColourValue Savable::loadColourValue(String name)
{
	Ogre::ColourValue value;
	tinyxml2::XMLElement* attr = mSaveGroup->FirstChildElement(name.c_str());
	if (attr)
	{
		value = Ogre::StringConverter::parseColourValue(attr->Attribute(XML_FILE_VALUE_NAME));
	}
	return value;
}


/*----------------------------------------------
	Helpers
----------------------------------------------*/

Quaternion Savable::directionFromString(Ogre::String quat)
{
	Quaternion result;
	if (quat == "LEFT")
		result = LEFT;
	else if (quat == "RIGHT")
		result = RIGHT;
	else if (quat == "TOP")
		result = TOP;
	else if (quat == "BOTTOM")
		result = BOTTOM;
	else if (quat == "FORWARD")
		result = FORWARD;
	else
		result = Ogre::StringConverter::parseQuaternion(quat);
	return result;
}
