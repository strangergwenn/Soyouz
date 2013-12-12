/**
* This work is distributed under the General Public License,
* see LICENSE for details
*
* @author Gwennaël ARBONA
**/

#ifndef __SAVABLE_H_
#define __SAVABLE_H_

#include "tinyxml2.hpp"
#include "Engine/game.hpp"


/*----------------------------------------------
	Class definitions
----------------------------------------------*/

class Savable
{

public:
	
	/**
	 * @brief Save the entire instance by dumping it to a XML file
	 * @return true if successful
	 **/
	bool saveToFile();
	
	/**
	 * @brief Load the instance from file
	 * @return true if successful
	 **/
	bool loadFromFile();


protected:

	/**
	 * @brief Override this to save your class
	 * @brief Call parent if inherited, call setSaveGroup(), then saveValue()
	 **/
	virtual void save() = 0;

	/**
	 * @brief Override this to load your class
	 * @brief Call parent if inherited, call setSaveGroup(), then getValue()
	 **/
	virtual void load() = 0;

	/**
	 * @brief Override this to set your file name
	 * @return the relative file name
	 **/
	virtual String getFileName() = 0;

	/**
	 * @brief Set your current class name
	 * @param name			Class name
	 **/
	void setSaveGroup(String name);

	/**
	 * @brief Save a value
	 * @param value			Value
	 * @param name			Value name
	 **/
	void saveValue(int value, String name);

	/**
	 * @brief Save a value
	 * @param value			Value
	 * @param name			Value name
	 **/
	void saveValue(float value, String name);

	/**
	 * @brief Save a value
	 * @param value			Value
	 * @param name			Value name
	 **/
	void saveValue(String value, String name);

	/**
	 * @brief Save a value
	 * @param value			Value
	 * @param name			Value name
	 **/
	void saveValue(Vector3 value, String name);

	/**
	 * @brief Save a value
	 * @param value			Value
	 * @param name			Value name
	 **/
	void saveValue(Quaternion value, String name);

	/**
	 * @brief Save a value
	 * @param value			Value
	 * @param name			Value name
	 **/
	void saveValue(Ogre::ColourValue value, String name);

	/**
	 * @brief Get a value from the save file
	 * @param name			Value name
	 * @return the value
	 **/
	int loadIntValue(String name);

	/**
	 * @brief Get a value from the save file
	 * @param name			Value name
	 * @return the value
	 **/
	float loadFloatValue(String name);

	/**
	 * @brief Get a value from the save file
	 * @param name			Value name
	 * @return the value
	 **/
	String loadStringValue(String name);

	/**
	 * @brief Get a value from the save file
	 * @param name			Value name
	 * @return the value
	 **/
	Vector3 loadVectorValue(String name);

	/**
	 * @brief Get a value from the save file
	 * @param name			Value name
	 * @return the value
	 **/
	Quaternion loadQuaternionValue(String name);

	/**
	 * @brief Get a value from the save file
	 * @param name			Value name
	 * @return the value
	 **/
	Ogre::ColourValue loadColourValue(String name);


protected:

	// Save data
	bool mIsSaving;
	tinyxml2::XMLDocument* mSaveFile;
	tinyxml2::XMLElement* mSaveData;
	tinyxml2::XMLElement* mSaveGroup;

};

#endif /* __SAVABLE_H_ */

