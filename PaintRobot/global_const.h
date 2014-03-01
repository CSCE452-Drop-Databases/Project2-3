/*
File: global_const.h

Authors: Colton Williams, Charles Kernstock, Chris Murray, Paul Gaughan, Wesley Tang
Department of Computer Science
Texas A&M University

*/

// GUI Constants
// Main window title
char* const WINDOW_TITLE = "Paint Robot";

// Control Panel Joint Section Titles
char* const cpjTitle = "Control Panel - Joint";
char* const cpjAxis1Title = "Axis 1";
char* const cpjAxis2Title = "Axis 2";
char* const cpjAxis3Title = "Axis 3";
char* const cpjPaintBrushTitle = "Paint Brush";
char* const cpjClearTitle = "Clear and Reset";
char* const cpjColorTitle = "Colors";

// Control Panel World Section Titles
char* const cpwTitle = "Control Panel - World";

int const WINDOW_WIDTH = 1000;
int const WINDOW_HEIGHT = 750;

// IMPORTANT NOTE: 3 * GUI_MARGIN_PERCENT + GUI_ROBOT_AREA_WIDTH_PERCENT + GUI_CONTROL_PANEL_WIDTH_PERCENT == 1.0
float const GUI_MARGIN_PERCENT = 0.025;
float const GUI_ROBOT_AREA_WIDTH_PERCENT = 0.725;
float const GUI_CONTROL_PANEL_WIDTH_PERCENT = 0.2;

// IMPORTANT NOTE: 3 * GUI_MARGIN_PERCENT + GUI_CONTROL_PANEL_JOINT_HEIGHT_PERCENT + GUI_CONTROL_PANEL_WORLD_HEIGHT_PERCENT == 1.0
float const GUI_CONTROL_PANEL_JOINT_HEIGHT_PERCENT = 0.60;
float const GUI_CONTROL_PANEL_WORLD_HEIGHT_PERCENT = 0.325;

// Slide that Axis1 moves along
int const SLIDE_LENGTH = 300;
float const SLIDE_LOCATION_PERCENT = 0.80; // percent from top of robot area slide

int const robotAreaWidth = WINDOW_WIDTH * GUI_ROBOT_AREA_WIDTH_PERCENT;
int const robotAreaHeight = WINDOW_HEIGHT * (1 - 2 * GUI_MARGIN_PERCENT);

int const controlPanelWidth = WINDOW_WIDTH * GUI_CONTROL_PANEL_WIDTH_PERCENT;
int const controlPanelJointHeight = GUI_CONTROL_PANEL_JOINT_HEIGHT_PERCENT * WINDOW_HEIGHT;
int const controlPanelWorldHeight = GUI_CONTROL_PANEL_WORLD_HEIGHT_PERCENT * WINDOW_HEIGHT;

int const controlPanelJointOriginX = WINDOW_WIDTH * (1 - GUI_MARGIN_PERCENT) - WINDOW_WIDTH * GUI_CONTROL_PANEL_WIDTH_PERCENT;
int const controlPanelJointOriginY = WINDOW_HEIGHT * GUI_MARGIN_PERCENT;

int const controlPanelWorldOriginX = WINDOW_WIDTH * (1 - GUI_MARGIN_PERCENT) - WINDOW_WIDTH * GUI_CONTROL_PANEL_WIDTH_PERCENT;
int const controlPanelWorldOriginY = controlPanelJointHeight + 2 * WINDOW_HEIGHT * GUI_MARGIN_PERCENT;

int const slidePosX = (robotAreaWidth - SLIDE_LENGTH) / 2; // where the slider should start in relation to the robot area
int const slidePosY = SLIDE_LOCATION_PERCENT * robotAreaHeight;


// Matrix constants
int const DEFAULT_MATRIX_SIZE = 4;

double const DEG_TO_RADS = 3.14159265 / 180.0;

int const MATRIX_VALUE_ONE = 1;
int const MATRIX_VALUE_ZERO = 0;

int const LINK_LENGTH_1 = 150;
int const LINK_LENGTH_2 = 100;
int const LINK_LENGTH_3 = 75;

int const ROTATE_AMOUNT = 1;
int const SLIDE_AMOUNT = 1;

int const LINK_WIDTH = 20;

int const PAINT_CIRCLE_RADIUS = 10;