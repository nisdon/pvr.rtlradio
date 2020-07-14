//---------------------------------------------------------------------------
// Copyright (c) 2020 Michael G. Brehm
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//---------------------------------------------------------------------------

#include "stdafx.h"
#include "dialogchannelscan.h"

#include <assert.h>

#include "string_exception.h"

#pragma warning(push, 4)

// channelsettings.xml
//
#define CONTROL_BUTTON_OK		1

//---------------------------------------------------------------------------
// dialogchannelscan Constructor (private)
//
// Arguments:
//
//	gui			- Pointer to the Kodi GUI library instance
//	window		- Pointer to the CAddonGUIWindow instance
//	scanner		- scanner object instance

dialogchannelscan::dialogchannelscan(CHelper_libKODI_guilib* gui, CAddonGUIWindow* window, std::unique_ptr<scanner> scanner)
	: m_gui(gui), m_window(window), m_scanner(std::move(scanner))
{
	assert(gui != nullptr);
	assert(window != nullptr);

	m_window->m_cbhdl = this;
	m_window->CBOnAction = onaction;
	m_window->CBOnClick = onclick;
	m_window->CBOnFocus = onfocus;
	m_window->CBOnInit = oninit;
}

//---------------------------------------------------------------------------
// dialogchannelscan Destructor

dialogchannelscan::~dialogchannelscan()
{
}

//---------------------------------------------------------------------------
// dialogchannelscan::show (static)
//
// Executes the window as a modal dialog box
//
// Arguments:
//
//	gui			- Pointer to the CHelper_libKODI_guilib interface
//	scanner		- scanner object instance

void dialogchannelscan::show(std::unique_ptr<CHelper_libKODI_guilib> const& gui, std::unique_ptr<scanner> scanner)
{
	CAddonGUIWindow*			window = nullptr;		// Kodi window instance

	assert(gui);
	assert(scanner);

	// Create the Kodi window instance
	window = gui->Window_create("channelscan.xml", "skin.estuary", false, true);
	if(window == nullptr) throw string_exception(__func__, ": failed to create Kodi Window object instance");

	try {

		// Create the dialog class instance
		std::unique_ptr<dialogchannelscan> dialog(new dialogchannelscan(gui.get(), window, std::move(scanner)));

		// Run the dialog as a modal dialog
		window->DoModal();

		// Destroy the dialog class instance
		dialog.reset(nullptr);

		// Destroy the Kodi window instance
		gui->Window_destroy(window);
	}

	// Destroy the window instance on any thrown exception
	catch(...) { gui->Window_destroy(window); throw; }
}

//---------------------------------------------------------------------------
// dialogchannelscan::onaction (static, private)
//
// Receives action codes that are sent to this window
//
// Arguments:
//
//	handle		- Window instance handle specified during construction
//	actionid	- Action code to be handled by the window

bool dialogchannelscan::onaction(GUIHANDLE handle, int actionid)
{
	assert(handle != nullptr);
	return reinterpret_cast<dialogchannelscan*>(handle)->onaction(actionid);
}

//---------------------------------------------------------------------------
// dialogchannelscan::onaction (private)
//
// Receives action codes that are sent to this window
//
// Arguments:
//
//	actionid	- Action code to be handled by the window

bool dialogchannelscan::onaction(int actionid)
{
	switch(actionid) {

		case ADDON_ACTION_CLOSE_DIALOG:
		case ADDON_ACTION_PREVIOUS_MENU:
		case ADDON_ACTION_NAV_BACK:
			return onclick(CONTROL_BUTTON_OK);
	}

	return false;
}

//---------------------------------------------------------------------------
// dialogchannelscan::onclick (static, private)
//
// Receives click events that are sent to this window
//
// Arguments:
//
//	handle		- Window instance handle specified during construction
//	controlid	- Control identifier receiving the click event

bool dialogchannelscan::onclick(GUIHANDLE handle, int controlid)
{
	assert(handle != nullptr);
	return reinterpret_cast<dialogchannelscan*>(handle)->onclick(controlid);
}

//---------------------------------------------------------------------------
// dialogchannelscan::onclick (private)
//
// Receives click events that are sent to this window
//
// Arguments:
//
//	controlid	- Control identifier receiving the click event

bool dialogchannelscan::onclick(int controlid)
{
	switch(controlid) {

		case CONTROL_BUTTON_OK:
			m_window->Close();
			return true;
	}

	return false;
}

//---------------------------------------------------------------------------
// dialogchannelscan::onfocus (static, private)
//
// Receives focus events that are sent to this window
//
// Arguments:
//
//	handle		- Window instance handle specified during construction
//	controlid	- Control identifier receiving the focus event

bool dialogchannelscan::onfocus(GUIHANDLE handle, int controlid)
{
	assert(handle != nullptr);
	return reinterpret_cast<dialogchannelscan*>(handle)->onfocus(controlid);
}

//---------------------------------------------------------------------------
// dialogchannelscan::onfocus (private)
//
// Receives focus events that are sent to this window
//
// Arguments:
//
//	controlid	- Control identifier receiving the focus event

bool dialogchannelscan::onfocus(int /*controlid*/)
{
	return true;
}

//---------------------------------------------------------------------------
// dialogchannelscan::oninit (static, private)
//
// Called to initialize the window object
//
// Arguments:
//
//	handle		- Window instance handle specified during construction

bool dialogchannelscan::oninit(GUIHANDLE handle)
{
	assert(handle != nullptr);
	return reinterpret_cast<dialogchannelscan*>(handle)->oninit();
}

//---------------------------------------------------------------------------
// dialogchannelscan::oninit (private)
//
// Called to initialize the window object
//
// Arguments:
//
//	NONE

bool dialogchannelscan::oninit(void)
{
	// warning cleanup
	if(m_gui == nullptr) return false;

	return true;
}

//---------------------------------------------------------------------------

#pragma warning(pop)
