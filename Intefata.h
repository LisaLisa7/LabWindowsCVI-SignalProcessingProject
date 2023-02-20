/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: OnPanel */
#define  PANEL_IDC_SWITCH_PANEL           2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  PANEL_LoadButton_2               3       /* control type: command, callback function: OnSs */
#define  PANEL_LoadButton                 4       /* control type: command, callback function: OnLoadButtonCB */
#define  PANEL_GRAPH_RAW_DATA             5       /* control type: graph, callback function: (none) */
#define  PANEL_START                      6       /* control type: numeric, callback function: (none) */
#define  PANEL_STOP                       7       /* control type: numeric, callback function: (none) */
#define  PANEL_IDC_MAX                    8       /* control type: numeric, callback function: (none) */
#define  PANEL_IDC_MIN                    9       /* control type: numeric, callback function: (none) */
#define  PANEL_GRAPH_FILTERED_DATA        10      /* control type: graph, callback function: (none) */
#define  PANEL_NUMERIC                    11      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_2                  12      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_3                  13      /* control type: numeric, callback function: (none) */
#define  PANEL_MIN_INDEX                  14      /* control type: numeric, callback function: (none) */
#define  PANEL_MAX_INDEX                  15      /* control type: numeric, callback function: (none) */
#define  PANEL_DISPERSIA                  16      /* control type: numeric, callback function: (none) */
#define  PANEL_GRAPH                      17      /* control type: graph, callback function: (none) */
#define  PANEL_FILTER_OPTION              18      /* control type: ring, callback function: (none) */
#define  PANEL_COMMANDBUTTON_PREV         19      /* control type: command, callback function: ON_PREV */
#define  PANEL_COMMANDBUTTON_NEXT         20      /* control type: command, callback function: ON_NEXT */
#define  PANEL_FILTER_BUTTON              21      /* control type: command, callback function: ON_FILTER */
#define  PANEL_INTERVAL_BUTTON            22      /* control type: command, callback function: ON_SPLIT */
#define  PANEL_NUMERICSLIDE               23      /* control type: scale, callback function: ON_ALPHA */
#define  PANEL_RINGSLIDE                  24      /* control type: slide, callback function: (none) */

#define  PANEL_2                          2       /* callback function: freq */
#define  PANEL_2_IDC_SWITCH_PANEL         2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  PANEL_2_LoadButton_2             3       /* control type: command, callback function: OnSs_2 */
#define  PANEL_2_GRAPH                    4       /* control type: graph, callback function: (none) */
#define  PANEL_2_GRAPH_6                  5       /* control type: graph, callback function: (none) */
#define  PANEL_2_GRAPH_5                  6       /* control type: graph, callback function: (none) */
#define  PANEL_2_GRAPH_4                  7       /* control type: graph, callback function: (none) */
#define  PANEL_2_GRAPH_2                  8       /* control type: graph, callback function: (none) */
#define  PANEL_2_GRAPH_3                  9       /* control type: graph, callback function: (none) */
#define  PANEL_2_WINDOW_OPTION            10      /* control type: ring, callback function: (none) */
#define  PANEL_2_IDC_FREQ_PEAK_3          11      /* control type: numeric, callback function: (none) */
#define  PANEL_2_IDC_POWER_PEAK_3         12      /* control type: numeric, callback function: (none) */
#define  PANEL_2_IDC_FREQ_PEAK_2          13      /* control type: numeric, callback function: (none) */
#define  PANEL_2_IDC_POWER_PEAK_2         14      /* control type: numeric, callback function: (none) */
#define  PANEL_2_RINGSLIDE                15      /* control type: slide, callback function: (none) */
#define  PANEL_2_IDC_FREQ_PEAK            16      /* control type: numeric, callback function: (none) */
#define  PANEL_2_IDC_POWER_PEAK           17      /* control type: numeric, callback function: (none) */
#define  PANEL_2_COMMANDBUTTON            18      /* control type: command, callback function: Panel2_GO */
#define  PANEL_2_COMMANDBUTTON_2          19      /* control type: command, callback function: FILTER_BUTTON */
#define  PANEL_2_Panel2Load               20      /* control type: command, callback function: onPanle2Load */
#define  PANEL_2_COMMANDBUTTON_3          21      /* control type: command, callback function: onSpectrum */
#define  PANEL_2_TIMER                    22      /* control type: timer, callback function: onTimer */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK FILTER_BUTTON(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK freq(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ON_ALPHA(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ON_FILTER(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ON_NEXT(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ON_PREV(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ON_SPLIT(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onPanle2Load(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onSpectrum(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSs(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSs_2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onTimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Panel2_GO(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif