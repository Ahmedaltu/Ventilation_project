/*
 * StringEdit.h
 *
 *  Created on: 20 Oct 2022
 *      Author: ahmed
 */

#ifndef STRINGEDIT_H_
#define STRINGEDIT_H_


#include "PropertyEdit.h"
#include "LiquidCrystal.h"
#include <string>
#include <atomic>
class StringEdit: public PropertyEdit {
public:
	StringEdit(LiquidCrystal *lcd_, std::string editTitle, std::string* items_, int n);
	virtual ~StringEdit();
	void increment();
	void decrement();
	void accept();
	void cancel();
	void setFocus(bool focus);
	bool getFocus();
	void display();
	int getValue();
	void setValue(int index);
	void toggle_adjust() {};

	static std::atomic<bool> string_changed;
private:
	void save();
	void displayEditValue();

	LiquidCrystal *lcd;
	std::string title;
	std::string* items;
	int value;
	int length;
	int edit;
	bool focus;
};



#endif /* STRINGEDIT_H_ */
