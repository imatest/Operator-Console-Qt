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

#include "passfailsettings.h"


PassFailSettings::PassFailSettings(void): b_isReadOnly(false)
{
	// and now we pay the price for hard-coding ...

	long long_bad_val = -1;
	double dbl_bad_val = -1.0;
	std::vector<long> long_bad_vec(3,-1);
    std::vector<double> dbl_bad_vec(3,-1);
	//////////////////////////////////////////////
	// sfrplus section
	//////////////////////////////////////////////
	sfrplus.All_Edge_IDs_detected.data_type.append("i");
    sfrplus.All_Edge_IDs_detected.name = "All_Edge_IDs_detected";
	sfrplus.All_Edge_IDs_detected.value = long_bad_val;
    sfrplus.All_Edge_IDs_detected.description = "Verify that all Edge_IDs that are specified in the edge ID file are detected";

	sfrplus.Bayer_decode.data_type.append("i");
    sfrplus.Bayer_decode.name ="Bayer_decode";
	sfrplus.Bayer_decode.value = long_bad_val;
    sfrplus.Bayer_decode.description = "Test for correct Bayer pattern decoding (fails if incorrect) if entered and the chart contains color patches above the center square and colorchart = 1 in the [sfrplus] section of the main ini file.";

	sfrplus.Chart_radial_pixel_shift_max.data_type.append("d");
    sfrplus.Chart_radial_pixel_shift_max.name = "Chart_radial_pixel_shift_max";
	sfrplus.Chart_radial_pixel_shift_max.value = dbl_bad_val;
    sfrplus.Chart_radial_pixel_shift_max.description = 	"Offset of chart center (from image center) in pixels.";

	sfrplus.Color_expected_detected.data_type.append("i");
    sfrplus.Color_expected_detected.name = "Color_expected_detected";
	sfrplus.Color_expected_detected.value = long_bad_val;
    sfrplus.Color_expected_detected.description = "A color pattern is expected above the chart center when colorchart = 1 in the [sfrplus] section of the main ini file. If the color pattern is not present, this test will fail. This could indicate bad chart positioning or simply that the color pattern is not present in the test chart.";

	sfrplus.DeltaE_00_mean_max.data_type.append("d");
    sfrplus.DeltaE_00_mean_max.name = "DeltaE_00_mean_max";
	sfrplus.DeltaE_00_mean_max.value = dbl_bad_val;
    sfrplus.DeltaE_00_mean_max.description = "Maximum allowed mean color difference between reference values and measured color values according to CIE Delta-E 2000 Standard";

	sfrplus.Convergence_angle_max.data_type.append("d");
    sfrplus.Convergence_angle_max.name = "Convergence_angle_max";
	sfrplus.Convergence_angle_max.value = dbl_bad_val;
    sfrplus.Convergence_angle_max.description = "The maximum allowed convergence angle (both Horizontal and Vertical are measured). A measure of keystone distortion.";

	sfrplus.FOV_degrees_diagonal_min.data_type.append("d");
    sfrplus.FOV_degrees_diagonal_min.name ="FOV_degrees_diagonal_min";
	sfrplus.FOV_degrees_diagonal_min.value = dbl_bad_val;
    sfrplus.FOV_degrees_diagonal_min.description = "The diagonal Field of View in degrees";

	sfrplus.Horizontal_bars_OK_min.data_type.append("i");
    sfrplus.Horizontal_bars_OK_min.name= "Horizontal_bars_OK_min";
	sfrplus.Horizontal_bars_OK_min.value = long_bad_val;
    sfrplus.Horizontal_bars_OK_min.description = "Test for properly detected top and bottom bars (present with no significant gaps).";

	sfrplus.Mirrored_chart.data_type.append("i");
    sfrplus.Mirrored_chart.name= "Mirrored_chart";
	sfrplus.Mirrored_chart.value = long_bad_val;
    sfrplus.Mirrored_chart.description = "Test for mirrored (Left-Right or Top-Bottom) image (fails mirrored images) if entered and the chart contains a grayscale step chart below the center square and stepchart = 1 in the [sfrplus] section of the main ini file.";

	sfrplus.Rotation_degrees_max.data_type.append("d");
    sfrplus.Rotation_degrees_max.name= "Rotation_degrees_max";
	sfrplus.Rotation_degrees_max.value = dbl_bad_val;
    sfrplus.Rotation_degrees_max.description = "Image rotation in degrees, measured from the top and bottom distortion bars. The absolute value is used for Pass/Fail. Note that if the chart is upside-down (vertically mirrored or rotated 180 degrees) 180 will be added to the rotation, which will generally result in a failure for this measurement.";

	sfrplus.Stepchart_expected_detected.data_type.append("i");
    sfrplus.Stepchart_expected_detected.name= "Stepchart_expected_detected";
	sfrplus.Stepchart_expected_detected.value = long_bad_val;
    sfrplus.Stepchart_expected_detected.description = "A grayscale stepchart is expected below the chart center when stepchart = 1 in the [sfrplus] section of the main ini file. If the stepchart is not present, this test will fail. This could indicate bad chart positioning or simply that the grayscale stepchart is not present in the test chart (a rare occurrence because it�s in most standard charts, except for the chrome-on-glass chart).";

	sfrplus.upside_down.data_type.append("i");
    sfrplus.upside_down.name= "upside_down";
	sfrplus.upside_down.value = long_bad_val;
    sfrplus.upside_down.description = "If set, fail the image if an upside-down condition is detected.";

	sfrplus.High_pixel_saturation_fraction_max.data_type.append("d");
    sfrplus.High_pixel_saturation_fraction_max.name= "High_pixel_saturation_fraction_max";
	sfrplus.High_pixel_saturation_fraction_max.value = dbl_bad_val;
    sfrplus.High_pixel_saturation_fraction_max.description = "Maximum fraction (of pixels) within 1% of maximum level (typically 255)";

	sfrplus.Low_pixel_saturation_fraction_max.data_type.append("d");
    sfrplus.Low_pixel_saturation_fraction_max.name= "Low_pixel_saturation_fraction_max";
	sfrplus.Low_pixel_saturation_fraction_max.value = dbl_bad_val;
    sfrplus.Low_pixel_saturation_fraction_max.description = "Maximum fraction (of pixels) within 2% of zero level";

	sfrplus.MTF50P_ratio_min.data_type.append("d");
    sfrplus.MTF50P_ratio_min.name= "MTF50P_ratio_min";
	sfrplus.MTF50P_ratio_min.value = dbl_bad_val;
    sfrplus.MTF50P_ratio_min.description = "The minimum ratio of the minimum to maximum MTF50P (spatial frequency where contrast falls to half its peak value). A low value indicates poor lens quality or possibly significant lens tilt.";

	sfrplus.MTF50P_CP_weighted_mean_min.data_type.append("d");
    sfrplus.MTF50P_CP_weighted_mean_min.name= "MTF50P_CP_weighted_mean_min";
	sfrplus.MTF50P_CP_weighted_mean_min.value = dbl_bad_val;
    sfrplus.MTF50P_CP_weighted_mean_min.description = "The weighted mean MTF50P in Cycles/Pixel.";

	sfrplus.Secondary_readout_1_center_mean_min.data_type.append("d");
    sfrplus.Secondary_readout_1_center_mean_min.name= "Secondary_readout_1_center_mean_min";
	sfrplus.Secondary_readout_1_center_mean_min.value = dbl_bad_val;
    sfrplus.Secondary_readout_1_center_mean_min.description = "The minimum allowed value of the mean of Secondary readout 1 in the center region (up to 30% of the center-to-corner distance). Secondary readouts are described Here.  Secondary readout ini file settings are listed in the Imatest INI file reference.";

	sfrplus.Secondary_readout_1_outer_mean_min.data_type.append("d");
    sfrplus.Secondary_readout_1_outer_mean_min.name= "Secondary_readout_1_outer_mean_min";
	sfrplus.Secondary_readout_1_outer_mean_min.value = dbl_bad_val;
    sfrplus.Secondary_readout_1_outer_mean_min.description = "The minimum allowed value of the mean of Secondary readout 1 in the outer region (over 30% of the center-to-corner distance). Note other regions are part-way, 30-75% and corner, >75%.";

	sfrplus.Secondary_readout_1_outer_min_min.data_type.append("d");
    sfrplus.Secondary_readout_1_outer_min_min.name= "Secondary_readout_1_outer_min_min";
	sfrplus.Secondary_readout_1_outer_min_min.value = dbl_bad_val;
    sfrplus.Secondary_readout_1_outer_min_min.description = "The minimum allowed value of the minimum value of Secondary readout 1 in the outer region";

	sfrplus.Secondary_readout_1_outer_quadrant_delta_max.data_type = "d";
    sfrplus.Secondary_readout_1_outer_quadrant_delta_max.name = "Secondary_readout_1_outer_quadrant_delta_max";
	sfrplus.Secondary_readout_1_outer_quadrant_delta_max.value = dbl_bad_val;
    sfrplus.Secondary_readout_1_outer_quadrant_delta_max.description = "";

	sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.data_type = "d";
    sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.name = "Secondary_readout_1_outer_quadrant_mean_min_min";
	sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.value = dbl_bad_val;
    sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.description = "";

	sfrplus.Secondary_readout_2_center_mean_min.data_type.append("d");
    sfrplus.Secondary_readout_2_center_mean_min.name= "Secondary_readout_2_center_mean_min";
	sfrplus.Secondary_readout_2_center_mean_min.value = dbl_bad_val;
    sfrplus.Secondary_readout_2_center_mean_min.description = "The minimum allowed value of the mean of Secondary readout 2 in the center region (up to 30% of the center-to-corner distance).";

	sfrplus.Secondary_readout_2_outer_mean_min.data_type.append("d");
    sfrplus.Secondary_readout_2_outer_mean_min.name= "Secondary_readout_2_outer_mean_min";
	sfrplus.Secondary_readout_2_outer_mean_min.value = dbl_bad_val;
    sfrplus.Secondary_readout_2_outer_mean_min.description = "The minimum allowed value of the mean of Secondary readout 2 in the outer region (over 30% of the center-to-corner distance)";

	sfrplus.Secondary_readout_2_outer_min_min.data_type.append("d");
    sfrplus.Secondary_readout_2_outer_min_min.name= "Secondary_readout_2_outer_min_min";
	sfrplus.Secondary_readout_2_outer_min_min.value = dbl_bad_val;
    sfrplus.Secondary_readout_2_outer_min_min.description = "The minimum allowed value of the minimum value of Secondary readout 2 in the outer region";

	sfrplus.Secondary_readout_2_outer_quadrant_delta_max.data_type = "d";
    sfrplus.Secondary_readout_2_outer_quadrant_delta_max.name = "Secondary_readout_2_outer_quadrant_delta_max";
	sfrplus.Secondary_readout_2_outer_quadrant_delta_max.value = dbl_bad_val;
    sfrplus.Secondary_readout_2_outer_quadrant_delta_max.description = "";

	sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.data_type = "d";
    sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.name = "Secondary_readout_2_outer_quadrant_mean_min_min";
	sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.value = dbl_bad_val;
    sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.description = "";

	sfrplus.Chart_mean_pixel_level_bounds.data_type.append("d");
    sfrplus.Chart_mean_pixel_level_bounds.group_name = "Chart_mean_pixel_level_bounds";
    sfrplus.Chart_mean_pixel_level_bounds.max_name= "Max";
	sfrplus.Chart_mean_pixel_level_bounds.max_val = dbl_bad_val;
    sfrplus.Chart_mean_pixel_level_bounds.max_description = "Maximum mean pixel levels for the chart, measured in the middle third (ninth by area) of the chart. Set to avoid cheating by over exposing so edges saturate (sharp corners), resulting in improved MTF.";
    sfrplus.Chart_mean_pixel_level_bounds.min_name= "Min";
	sfrplus.Chart_mean_pixel_level_bounds.min_val = dbl_bad_val;
    sfrplus.Chart_mean_pixel_level_bounds.min_description = "Minimum mean pixel levels for the chart, measured in the middle third (ninth by area) of the chart. Set to avoid cheating by under exposing so edges saturate (sharp corners), resulting in improved MTF.";

	sfrplus.passfail_ini_file_date.data_type.append("");
    sfrplus.passfail_ini_file_date.name= "passfail_ini_file_date";
    sfrplus.passfail_ini_file_date.value = "";

#ifdef LOWER_CASE_INI_SECTIONS
    sfrplus.name= "sfrplus";
#else
    sfrplus.name= "SFRplus";
#endif
//    sfrplus.numEntries = 27;
    sfrplus.numEntries = 25;    //Spec-v2.ini has 25 entries
    sfrplus.b_enable = false;
	//////////////////////////////////////////////
	// blemish section
	//////////////////////////////////////////////

	blemish.Dead_pixels_max.data_type = "i";
    blemish.Dead_pixels_max.name= "Dead_pixels_max";
	blemish.Dead_pixels_max.value = long_bad_val;
    blemish.Dead_pixels_max.description = "Maximum number of dead pixels allowed. Thresholds for dead pixel detection (% or absolute) are set in the [blemish] section of the ini file.";

	blemish.Dead_pixel_clusters_max.data_type = "i";
    blemish.Dead_pixel_clusters_max.name= "Dead_pixel_clusters_max";
	blemish.Dead_pixel_clusters_max.value = long_bad_val;
    blemish.Dead_pixel_clusters_max.description = "Maximum number of dead pixel clusters allowed, where a cluster is defined as two or more adjacent dead pixels. Overrides the calcluster setting in the standard ini file: calcluster(2) is set to 1 if entered.";

	blemish.Defective_pixels_max_count.data_type = "i";
    blemish.Defective_pixels_max_count.name = "Defective_pixels_max_count";
	blemish.Defective_pixels_max_count.value = long_bad_val;
    blemish.Defective_pixels_max_count.description = "";

	blemish.Hot_pixel_clusters_max.data_type = "i";
    blemish.Hot_pixel_clusters_max.name = "Hot_pixel_clusters_max";
	blemish.Hot_pixel_clusters_max.value = long_bad_val;
    blemish.Hot_pixel_clusters_max.description = "";

	blemish.Hot_pixels_max.data_type = "i";
    blemish.Hot_pixels_max.name= "Hot_pixels_max";
	blemish.Hot_pixels_max.value = long_bad_val;
    blemish.Hot_pixels_max.description = "Maximum number of hot pixels allows. Thresholds for hot pixel detection (% or absolute) are set in the [blemish] section of the ini file.";

	blemish.Optical_center_offset_max.data_type = "d";
    blemish.Optical_center_offset_max.name= "Optical_center_offset_max";
	blemish.Optical_center_offset_max.value = dbl_bad_val;
    blemish.Optical_center_offset_max.description = "Maximum offset in pixels of the optical center (location of highest luminance) from the center of the image. To minimize the effects of noise, X and Y optical centers are defined using the centroid of the of the curve between the points where the smoothed values fall below 95% of the maximum. (The actual maximum is highly susceptible to noise since it�s very broad.)";

	blemish.Optical_center_offset_X_max.data_type = "d";
    blemish.Optical_center_offset_X_max.name = "Optical_center_offset_X_max";
	blemish.Optical_center_offset_X_max.value = dbl_bad_val;
    blemish.Optical_center_offset_X_max.description = "";

	blemish.Optical_center_offset_Y_max.data_type = "d";
    blemish.Optical_center_offset_Y_max.name = "Optical_center_offset_Y_max";
	blemish.Optical_center_offset_Y_max.value = dbl_bad_val;
    blemish.Optical_center_offset_Y_max.description = "";

	blemish.Relative_illumination_corner_diff_pct_max.data_type = "d";
    blemish.Relative_illumination_corner_diff_pct_max.name = "Relative_illumination_corner_diff_pct_max";
	blemish.Relative_illumination_corner_diff_pct_max.value = dbl_bad_val;
    blemish.Relative_illumination_corner_diff_pct_max.description = "";

	blemish.Relative_illumination_worst_corner_pct_min.data_type = "d";
    blemish.Relative_illumination_worst_corner_pct_min.name= "Relative_illumination_worst_corner_pct_min";
	blemish.Relative_illumination_worst_corner_pct_min.value = dbl_bad_val;
    blemish.Relative_illumination_worst_corner_pct_min.description = "The minimum corner region luminance divided by the maximum luminance of the smoothed image.";

	blemish.Uniformity_RoverG_corners_pct_max.data_type = "d";
    blemish.Uniformity_RoverG_corners_pct_max.name= "Uniformity_RoverG_corners_pct_max";
	blemish.Uniformity_RoverG_corners_pct_max.value = long_bad_val;
    blemish.Uniformity_RoverG_corners_pct_max.description = "Maximum R/G corner non-uniformity = 100%(RGmax � RGmin)/RGmax, where RGmax = maximum R/G ratio in the four corners; RGmin = minimum R/G ratio in the four corners.";

	blemish.Uniformity_BoverG_corners_pct_max.data_type = "d";
    blemish.Uniformity_BoverG_corners_pct_max.name= "Uniformity_BoverG_corners_pct_max";
	blemish.Uniformity_BoverG_corners_pct_max.value = long_bad_val;
    blemish.Uniformity_BoverG_corners_pct_max.description = "Maximum B/G corner non-uniformity = 100%(BGmax � BGmin)/BGmax, where BGmax = maximum B/G ratio in the four corners; BGmin = minimum B/G ratio in the four corners.";

	blemish.Blemish_size_pixels.data_type = "i";
    blemish.Blemish_size_pixels.group_name = "Blemish_size_pixels";
    blemish.Blemish_size_pixels.group_description = "Sizes of blemishes in pixels for Blemish_maximum_count, below";
	blemish.Blemish_size_pixels.value = long_bad_vec;

	blemish.Blemish_maximum_count.data_type = "i";
    blemish.Blemish_maximum_count.group_name = "Blemish_maximum_count";
    blemish.Blemish_maximum_count.group_description = "Maximum number of blemishes for pixels with maximum sizes specified by Blemish_size_pixels";
	blemish.Blemish_maximum_count.value =long_bad_vec;

#ifdef LOWER_CASE_INI_SECTIONS
    blemish.name= "blemish";
#else
    blemish.name= "Blemish";
#endif
//    blemish.numEntries = 14;
    blemish.numEntries = 11;    // Spec-v2.ini has 11 Blemish entries
    blemish.b_enable = false;

	//////////////////////////////////////////////
	// ois section
	//////////////////////////////////////////////

	ois.L_MTF50_delta2_gain_summary_all_dB_min.data_type = "i";
    ois.L_MTF50_delta2_gain_summary_all_dB_min.name= "L_MTF50_delta2_gain_summary_all_dB_min";
	ois.L_MTF50_delta2_gain_summary_all_dB_min.value = long_bad_val;

	ois.R_improve_H_dB_min.data_type = "i";
    ois.R_improve_H_dB_min.name= "R_improve_H_dB_min";
    ois.R_improve_H_dB_min.value = long_bad_val;

	ois.R_improve_V_dB_min.data_type = "i";
    ois.R_improve_V_dB_min.name= "R_improve_V_dB_min";
	ois.R_improve_V_dB_min.value = long_bad_val;

	ois.R_improve_ALL_dB_min.data_type = "i";
    ois.R_improve_ALL_dB_min.name= "R_improve_ALL_dB_min";
	ois.R_improve_ALL_dB_min.value = long_bad_val;

#ifdef LOWER_CASE_INI_SECTIONS
    ois.name = "OIS");
#else
    ois.name = "ois";
#endif
	ois.numEntries = 4;
	ois.b_enable = false;

	//////////////////////////////////////////////
	// other section
	//////////////////////////////////////////////

	other.b_enable = false;

}


PassFailSettings::~PassFailSettings(void)
{
}
