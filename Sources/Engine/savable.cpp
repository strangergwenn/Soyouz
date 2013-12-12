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

const Ogre::String XML_FILE_DIR = "Content/Data/Save/0000/";
const char* XML_FILE_ROOT_NAME = "save";
const char* XML_FILE_VALUE_NAME = "value";
const char* XML_FILE_HEADER = "xml version=\"1.0\" encoding=\"UTF-8\"";


/*----------------------------------------------
	Public methods
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
		mSaveData = mSaveFile->NewElement(XML_FILE_ROOT_NAME);

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
			mSaveData = mSaveFile->FirstChildElement(XML_FILE_ROOT_NAME);
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
	Protected methods
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
