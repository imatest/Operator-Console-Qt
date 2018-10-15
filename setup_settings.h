/****************************************************************************	
*	Operator Console - an extensible user interface for the Imatest IT 		*
*	library																	*
*	Copyright (C) 2013 Imatest LLC.											*
*																			*
*	This program is free software: you can redistribute it and/or modify	*
*	it under the terms of the GNU General Public License as published by	*
*	the Free Software Foundation, either version 3 of the License, or		*
*	(at your option) any later version.										*
*																			*
*	This program is distributed in the hope that it will be useful,			*
*	but WITHOUT ANY WARRANTY; without even the implied warranty of			*
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the			*
*	GNU General Public License for more details.							*
*																			*
*	You should have received a copy of the GNU General Public License		*
*	along with this program.  If not, see <http://www.gnu.org/licenses/>. 	*
****************************************************************************/

#ifndef GUARD_SETUP_SETTINGS_H
#define GUARD_SETUP_SETTINGS_H

#include <QVector>
#include <QCameraInfo>

typedef enum EDevIndex
{
    eAptina = 0,
    eOmnivision,
    eToshiba,
    eSTM,
    eGraphin,
    eEpiphan,
    eQCamera,
    eLoadFile,
    eNumDevIndex
} EDevIndex;

///
/// A struct that contains the various data controlled by the setup dialog.
///
struct setup_settings
{
    int               	width;					//!< image width in pixels
    int                	height;					//!< image height in pixels
    int                 bits_per_pixel;			//!< Used for Omnivision only: number of bits per pixel (8,10,12 only)
    int                 epiphan_deviceID;		//!< Used for Epiphan only: the device ID (0 or 1 only)
    int                 sourceID;				//!< sourceID for acquire_image()
                                                /**< Only takes values 1 through 6. 1 => Aptina DevWare, 2 => Omnivision, 3 => Toshiba, 4 => STM Conduit, 5 => Graphin EasyLab, 6 => Epiphan*/
    int                 bayer;					//!< Used for Omnivision only: Bayer pattern index
                                                /**< Only takes values 1 through 4.  1 => Red in R1C1, 2=> Red in R1C2, 3=> Red in R2C1, 4=> Red in R2C2 */
    QString             qcam_deviceID;          //!< Used for QCamera only: this is a unique ID to identify the camera (may not be human-readable)
    QString             omnivision_reg_file;	//!< [OPTIONAL INPUT] Used for Omnivision only: a fully-qualified filename (including full path) for a file containing camera register settings
    QString             part_number;			//!< [OPTIONAL INPUT] The user supplied camera part number to be used in the JSON output
    QString             serial_number;			//!< [OPTIONAL INPUT] The user supplied camera part serial to be used in the JSON output
    QString             ini_file;				//!< The fully-qualified file name (including full path) for the imatest.ini file
    QString             chart_def_file;		    //!< The fully-qualified file name (including full path) for the arbitrary chart definition file
    QString             program_path;			//!< The Imatest IT program path
    QVector<QString>    epiphan_deviceID_list;	//!< The list of allowed Epiphan device IDs used to fill the c_combo_deviceid CComboBox in Setup.h/cpp
    QVector<QString>	device_list;			//!< The list of allowed device names used to fill the c_device_list CListBox in Setup.h/cpp
    QVector<QString>	bayer_list;				//!< The list of allowed Bayer patterns used to fill the c_combo_bayer CComboBox in Setup.h/cpp
    QVector<int>        allowed_bits_per_pixel; //!< The list of allowed bits-per-pixels used to fill the c_combo_bits_per_pixel CComboBox in Setup.h/cpp
    QString             qcam_description;       //!< Human-readable decription of the QCamera


    ///
	/// default constructor
	///

    setup_settings(void): width(0), height(0), bits_per_pixel(0), epiphan_deviceID(1), sourceID(6), bayer(0) {
		
		/////////////////////////////////////////////////////
		//
		// Do not change these vectors without updating 
		// the functions that rely upon them in Setup.cpp,CSetup::OnLbnSelchangeDeviceList() and CSetup::OnInitDialog()!!!!
		//
		/////////////////////////////////////////////////////
        epiphan_deviceID_list.resize(2);
        epiphan_deviceID_list[0] = "0";
        epiphan_deviceID_list[1] = "1";

        device_list.resize(EDevIndex::eNumDevIndex);
        device_list[eAptina]     = "Aptina DevWare";
        device_list[eOmnivision] = "Omnivision OVTA";
        device_list[eToshiba]    = "Toshiba ImaTuning";
        device_list[eSTM]        = "STM Conduit";
        device_list[eGraphin]    = "Graphin EasyLab";
        device_list[eEpiphan]    = "Epiphan";
        device_list[eQCamera]    = "DirectShow camera";
        device_list[eLoadFile]   = "Load file";

        bayer_list.resize(4);
        bayer_list[0] = "Red in R1C1 (RG/GB)";
        bayer_list[1] = "Red in R1C2 (GR/BG)";
        bayer_list[2] = "Red in R2C1 (GB/RG)";
        bayer_list[3] = "Red in R2C2 (BG/GR)";

        allowed_bits_per_pixel.resize(1);
        allowed_bits_per_pixel[0] = 8; // only 1-byte pixels for now
		//allowed_bits_per_pixel.resize(3,8);
		//allowed_bits_per_pixel[1] = 10;
		//allowed_bits_per_pixel[2] = 12;
    }

};

#endif
