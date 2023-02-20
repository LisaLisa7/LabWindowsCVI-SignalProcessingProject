#include <utility.h>
#include <analysis.h>
#include <ansi_c.h>
#include <advanlys.h>
#include <formatio.h>
#include <cvirte.h>		
#include <userint.h>
#include "Intefata.h"

// Constants
#define SAMPLE_RATE		0
#define NPOINTS			1

//==============================================================================
// Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;
double *waveData = 0;
double *filt =0;
double *anvelope = 0;
double alpha=0.1;

int start = 0;
int stop = 0;
static int nr_poze_salvate=1;
WindowConst win_SG,win_notch,win_raw;

//

int numar_puncte;
double * sgFilter;
double* notchFilter;
//


static int panelHandle;

//int wavePanel = 0;
int freqPanel = 0;


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "Intefata.uir", PANEL)) < 0)
		return -1;
	if ((freqPanel = LoadPanel (0, "Intefata.uir", PANEL_2)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	
	DiscardPanel (panelHandle);
	
	return 0;
}

int CVICALLBACK OnLoadButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	
	
	double min = 0.0;
	double max = 0.0;
	int MaxIndex = 0;
	int MinIndex = 0;
	double mediana = 0.0;
	double media=0.0;
	double intersectii = 0;
	double m2=0.0;
	
	static ssize_t hist[101];
    static double axis[101];
	
	int img;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			//GENERARE
			//executa script python pentru conversia unui fisierului .wav in .txt
			//LaunchExecutable("python main.py");
			
			//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
			Delay(4);
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			anvelope = (double *) calloc(npoints,sizeof(double));
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			FileToArray("anvelopeAmplitude.txt", anvelope, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			//afisare pe grapf
			PlotY(panel, PANEL_GRAPH_RAW_DATA, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, 0x00FFCCFF);
			
			//Add1D(waveData,anvelope,npoints,waveData);
			
			//afisare pe acelasi graf anvelopa
			PlotY(panel, PANEL_GRAPH_RAW_DATA, anvelope, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS,0x00CCFFFF );
			
			
			// MAX,MIN,MEDIE
			
			MaxMin1D(waveData, npoints, &max, &MaxIndex, &min, &MinIndex);
			Mean(waveData, npoints, &media);
			SetCtrlVal(panel, PANEL_IDC_MIN, min);
			SetCtrlVal(panel, PANEL_IDC_MAX, max);
			SetCtrlVal(panel, PANEL_NUMERIC_2, media);
			
			double* SortedWaveData = 0;
			SortedWaveData = (double *) calloc(npoints, sizeof(double));
			
			
			// calculez intersectiile cu 0 si dispersia
			double suma2=0;
			for(int i=0;i<npoints;i++){
				if(waveData[i] == 0)
					intersectii++;
				SortedWaveData[i] = waveData[i];
				suma2=suma2+(waveData[i] - media)*(waveData[i] - media);
			}
			
			m2=suma2/npoints;
			
			SetCtrlVal(panel, PANEL_DISPERSIA, sqrt(m2));
			SetCtrlVal(panel,PANEL_NUMERIC_3,intersectii);
			SetCtrlVal(panel,PANEL_MIN_INDEX,MinIndex);
			SetCtrlVal(panel,PANEL_MAX_INDEX,MaxIndex);
			
			
			//aflu mediana
			Sort(SortedWaveData,npoints,ANALYSIS_SORT_ASCENDING,SortedWaveData);
			mediana = SortedWaveData[npoints/2];
			SetCtrlVal(panel, PANEL_NUMERIC, mediana);
			
			
			//Histograma
			
			double intervals = 1+3.3*log(npoints); // Sturge's rule
			// https://www.ni.com/docs/en-US/bundle/labwindows-cvi/page/advancedanalysisconcepts/lvac_histogram.html?fbclid=IwAR2Y0EnPua6pzUSRK9YHxNPJYtbMKvq-2e9qMNTEFpM-EzpXSTr7CcEyaiI
			Histogram(waveData,npoints,min,max,hist,axis,intervals);
			PlotXY (panel, PANEL_GRAPH, axis,  hist,intervals, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, 0x00FFCCFF);
			
			
			//Salvare ss
			
			GetCtrlDisplayBitmap(panel,PANEL_GRAPH_RAW_DATA,1,&img);
			SaveBitmapToJPEGFile(img,"RawData.jpg",JPEG_DCTFLOAT,100);
			DiscardBitmap(img);
			
			break;
	}
	return 0;
}


int CVICALLBACK OnPanel (int panel, int event, void *callbackData,
						 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			if(waveData!=NULL)
				free(waveData);
			if(filt !=NULL)
				free(filt);
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK ON_NEXT (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(start==0 && stop!=0)
				start++;
			if(start==0 && stop == 0)
				stop++;
			
			if(start!=0 && stop!=0)
			{
				start = stop;
				stop++;
			}
			SetCtrlVal(panel,PANEL_STOP,stop);
			SetCtrlVal(panel,PANEL_START,start);
			break;
	}
	return 0;
}

int CVICALLBACK ON_PREV (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			if(start!=0)
			{
				start--;
				stop--;
			}
			SetCtrlVal(panel,PANEL_START,start);
			SetCtrlVal(panel,PANEL_STOP,stop);
			
			
			break;
	}
	return 0;
}

double * ordin1(double array [], int npoints, double alpha)
{
	int i;
	filt=(double*)calloc(npoints,sizeof(double));
	filt[0]=array[0];
	for(i=1;i<npoints;i++)
	{
		filt[i]=(1-alpha)*filt[i-1]+alpha*array[i];
	}
	return filt;
}

int CVICALLBACK ON_FILTER (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	int val=0;
	int el_med;
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(panel,PANEL_FILTER_OPTION,&val);
			GetCtrlVal(panel,PANEL_NUMERICSLIDE,&alpha);
			GetCtrlVal(panel,PANEL_RINGSLIDE,&el_med);
			if(val == 1)
			{
				DeleteGraphPlot(panel,PANEL_GRAPH_FILTERED_DATA,-1,VAL_IMMEDIATE_DRAW);
				
				if(filt != NULL)
					free(filt);
				
				filt= (double *) calloc(npoints, sizeof(double));
				filt = ordin1(waveData,npoints,alpha);
				PlotY(panel,PANEL_GRAPH_FILTERED_DATA, filt,npoints-1, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, 0x00FFCCFF);
				
			}
			else
			{
				
				DeleteGraphPlot(panel,PANEL_GRAPH_FILTERED_DATA,-1,VAL_IMMEDIATE_DRAW);
				if(filt!=NULL)
					free(filt);
				filt= (double *) calloc(npoints, sizeof(double));
				if(el_med == 32)
					MedianFilter(waveData,npoints,32,31,filt);
				else if(el_med == 16)
					MedianFilter(waveData,npoints,16,15,filt);
				PlotY(panel,PANEL_GRAPH_FILTERED_DATA, filt,npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, 0x00FFCCFF);
				
			}

			break;
	}
	return 0;
}

int CVICALLBACK ON_SPLIT (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	int left,right;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(panel,PANEL_START,&left);
			GetCtrlVal(panel,PANEL_STOP,&right);
			SetAxisRange(panel,PANEL_GRAPH_FILTERED_DATA,VAL_MANUAL,left*waveInfo[0],right*waveInfo[0],VAL_AUTOSCALE,1,2);
			
			

			break;
	}
	return 0;
}

int CVICALLBACK OnSs (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	int img;
	char path_poza[100];
	int start_interval,stop_interval;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(panel,PANEL_START,&start_interval);
			GetCtrlVal(panel,PANEL_STOP,&stop_interval);
			
			GetCtrlDisplayBitmap(panel,PANEL_GRAPH_FILTERED_DATA,1,&img);
			sprintf(path_poza,"FilteredDataNr%d_Interval[%d,%d].jpg",nr_poze_salvate,start_interval,stop_interval);
			SaveBitmapToJPEGFile(img,path_poza,JPEG_DCTFLOAT,100);
			nr_poze_salvate++;
			
			DiscardBitmap(img);
			break;
	}
	return 0;
}

int CVICALLBACK ON_ALPHA (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			GetCtrlVal( panel, control, &alpha);
			break;
	}
	return 0;
}

int CVICALLBACK OnSwitchPanelCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			if(panel == panelHandle)
			{
				SetCtrlVal(freqPanel, PANEL_2_IDC_SWITCH_PANEL, 1);
				DisplayPanel(freqPanel);
				HidePanel(panel);
			}
			else
			{
				SetCtrlVal(panelHandle,PANEL_2_IDC_SWITCH_PANEL, 0);
				DisplayPanel(panelHandle);
				HidePanel(panel);
			}
			break;
			
	}
	return 0;
}

int CVICALLBACK freq (int panel, int event, void *callbackData,
					  int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);

			break;
	}
	return 0;
}

int CVICALLBACK Panel2_GO (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	int window_type;
	int numar_puncte; // ales de pe interfata
	
	switch (event)
	{
		case EVENT_COMMIT:			
			
			GetCtrlVal(panel,PANEL_2_WINDOW_OPTION,&window_type);
			GetCtrlVal(panel,PANEL_2_RINGSLIDE,&numar_puncte);
			if(window_type == 0)
			{
				ScaledWindowEx(waveData,numar_puncte,GAUSSIAN,-1,&win_raw);
				ScaledWindowEx(sgFilter,numar_puncte,GAUSSIAN,-1,&win_SG);
				ScaledWindowEx(notchFilter,numar_puncte,GAUSSIAN,-1,&win_notch);
				
				//SG FILTER
				DeleteGraphPlot(panel,PANEL_2_GRAPH_2,-1,VAL_IMMEDIATE_DRAW);
				PlotY(panel,PANEL_2_GRAPH_2,sgFilter,numar_puncte, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_GREEN);
				
				
				// NOTCH FILTER
				
				DeleteGraphPlot(panel,PANEL_2_GRAPH_3,-1,VAL_IMMEDIATE_DRAW);
				PlotY(panel,PANEL_2_GRAPH_3,notchFilter,numar_puncte, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				
				//WINDOWED RAW SIGNAL
				DeleteGraphPlot(panel,PANEL_2_GRAPH,-1,VAL_IMMEDIATE_DRAW);
				PlotY(panel,PANEL_2_GRAPH,waveData, numar_puncte, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, 0x00FFCCFF);
				
			}
			else
			{
				//
				
				ScaledWindowEx(waveData,numar_puncte,RECTANGLE,1,&win_raw);
				ScaledWindowEx(sgFilter,numar_puncte,RECTANGLE,-1,&win_SG);
				ScaledWindowEx(notchFilter,numar_puncte,RECTANGLE,-1,&win_notch);
				
				//SG FILTER
				DeleteGraphPlot(panel,PANEL_2_GRAPH_2,-1,VAL_IMMEDIATE_DRAW);
				PlotY(panel,PANEL_2_GRAPH_2,sgFilter,numar_puncte, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_GREEN);
				
				// NOTCH FILTER
				DeleteGraphPlot(panel,PANEL_2_GRAPH_3,-1,VAL_IMMEDIATE_DRAW);
				PlotY(panel,PANEL_2_GRAPH_3,notchFilter,numar_puncte, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_RED);
				
				
				//WINDOWED RAW SIGNAL
				DeleteGraphPlot(panel,PANEL_2_GRAPH,-1,VAL_IMMEDIATE_DRAW);
				PlotY(panel,PANEL_2_GRAPH, waveData, numar_puncte, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, 0x00FFCCFF);
			}
			

			break;
	}
	return 0;
}

int CVICALLBACK FILTER_BUTTON (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	double Numerator[3];
	double Denominator[3];
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(panel,PANEL_2_RINGSLIDE,&numar_puncte);
			sgFilter = (double *) calloc(numar_puncte, sizeof(double));
			notchFilter = (double *) calloc(numar_puncte, sizeof(double));
			
			// SG FILTER
			SavitzkyGolayFiltering(waveData,numar_puncte,3,9,NULL,sgFilter);
			
			
			
			// NOTCH
			
			//coef filtrului Notch pentru frec de 900Hz
			// obtinuti cu ajutorul:  https://www.mathworks.com/help/dsp/ref/iirnotch.html
			/*
			Numerator[0]=0.994051538502345;
			Numerator[1]=-1.816222535322912;
			Numerator[2]=0.994051538502345;
			Denominator[0]=1; 
			Denominator[1]=-1.816222535322912;
			Denominator[2]=0.988103077004690;
			*/
			
			
			Numerator[0]=0.993755964953657;
			Numerator[1]= 0.614174962864309 ;
			Numerator[2]=0.993755964953657;
			Denominator[0]=1; 
			Denominator[1]= 0.614174962864309;
			Denominator[2]=0.987511929907314;
			
			notchFilter[0]=waveData[0]; notchFilter[1]=waveData[1];
			for(i=2;i<numar_puncte;i++) //dimensiunea bufferului =1024 sau se inlocuieste 
			//cu numarul de esantioane
				notchFilter[i]=Numerator[0]*waveData[i]+Numerator[1]*waveData[i-1]+Numerator[2]*waveData[i-2] -Denominator[1]*notchFilter[i-1]-Denominator[2]*notchFilter[i-2];
			
			
			
			
			
	}
	return 0;
}

int CVICALLBACK onPanle2Load (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			waveData = (double *) calloc(npoints, sizeof(double));
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			break;
	}
	return 0;
}

int CVICALLBACK onSpectrum (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	
	int window_type;
	int numar_puncte; // ales de pe interfata
	
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			SetCtrlAttribute(freqPanel,PANEL_2_TIMER,ATTR_ENABLED,1);
			
			GetCtrlVal(panel,PANEL_2_WINDOW_OPTION,&window_type);
			GetCtrlVal(panel,PANEL_2_RINGSLIDE,&numar_puncte);
			
			//determinare spectru de putere
	
			//vector ce contine spectrul semnalului convertit la volti
			double convertedSpectrum[numar_puncte/2];
			//frecventa estimata pentru spectrul de putere (maxim) din vectorul autoSpectrum
			double powerPeak = 0.0;//
			//valoarea maxima din spectru de putere (din autoSpectrum)
			double freqPeak = 0.0;
			//variabila ce reprezinta pasul in domeniul frecventei
			double df = 0.0;
			//voltage signal - descriere a semnalului achizitionat
			char unit[32] = "V";
			//spectrul de putere cu un numar de valori egal cu jumatate din dimensiunea bufferuluide intrare
			double autoSpectrum[numar_puncte/2];
			//
			
			
			//partea pozitiva a spectrului scalat de putere pentru un semnal esantionat
			AutoPowerSpectrum(waveData,numar_puncte,1./numar_puncte,autoSpectrum,&df);
			//calculeaza puterea si frecventa corespunzatoare varfului din spectrul semnalului  
		
			PowerFrequencyEstimate(autoSpectrum,numar_puncte/2,-1,win_raw,df,7,&freqPeak,&powerPeak);
		
			SpectrumUnitConversion(autoSpectrum,numar_puncte/2,SPECTRUM_POWER,SCALING_MODE_LINEAR,DISPLAY_UNIT_VPK,df,win_raw,convertedSpectrum,unit);
			SetCtrlVal(panel, PANEL_2_IDC_FREQ_PEAK, freqPeak);
			SetCtrlVal(panel, PANEL_2_IDC_POWER_PEAK, powerPeak);
	    	//Functia converteste spectrul de intrare (care este puterea, amplitudinea sau amplificarea) 
			//în formate alternative (linear, logarithmic, dB) ce permit o reprezentare grafica mai convenabila.
			//power peak estimeaza vrms^2
		
			//sterg graph-ul unde urmeaza sa plotez spectrul semnalului
			DeleteGraphPlot (panel, PANEL_2_GRAPH_4, -1, VAL_IMMEDIATE_DRAW);	
			//plotez spectrul semnalului
	   	 	PlotWaveform(panel, PANEL_2_GRAPH_4, convertedSpectrum, numar_puncte/2 ,VAL_DOUBLE, 1.0, 0.0, 0.0, df,
                                    VAL_FAT_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, 0x00FFCCFF);
			//PlotY(mainPanel, MAIN_PANEL_IDC_OUTGRAPH_2, Converted_Spec, NB_PTS/4, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_DK_GREEN);

			
			
			//pt filtrat S-G
			
			
			double convertedSpectrum_F[numar_puncte/2];
			double powerPeak_F = 0.0;//
			double freqPeak_F = 0.0;
			double df_F = 0.0;
			double autoSpectrum_F[numar_puncte/2];
			AutoPowerSpectrum(sgFilter,numar_puncte,1./numar_puncte,autoSpectrum_F,&df_F);
			PowerFrequencyEstimate(autoSpectrum_F,numar_puncte/2,-1,win_SG,df_F,7,&freqPeak_F,&powerPeak_F);
			SpectrumUnitConversion(autoSpectrum_F,numar_puncte/2,SPECTRUM_POWER,SCALING_MODE_LINEAR,DISPLAY_UNIT_VPK,df_F,win_SG,convertedSpectrum_F,unit);
			SetCtrlVal(panel, PANEL_2_IDC_FREQ_PEAK_2, freqPeak_F);
			SetCtrlVal(panel, PANEL_2_IDC_POWER_PEAK_2, powerPeak_F);
			DeleteGraphPlot (panel, PANEL_2_GRAPH_5, -1, VAL_IMMEDIATE_DRAW);	
	   	 	PlotWaveform(panel, PANEL_2_GRAPH_5, convertedSpectrum_F, numar_puncte/2 ,VAL_DOUBLE, 1.0, 0.0, 0.0, df_F,
                                    VAL_FAT_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
			
			//pt filtrat NOTCH
			
			
			double convertedSpectrum_F2[numar_puncte/2];
			double powerPeak_F2 = 0.0;//
			double freqPeak_F2 = 0.0;
			double df_F2 = 0.0;
			double autoSpectrum_F2[numar_puncte/2];
			AutoPowerSpectrum(notchFilter,numar_puncte,1./numar_puncte,autoSpectrum_F2,&df_F2);
			PowerFrequencyEstimate(autoSpectrum_F2,numar_puncte/2,-1,win_notch,df_F2,7,&freqPeak_F2,&powerPeak_F2);
			SpectrumUnitConversion(autoSpectrum_F2,numar_puncte/2,SPECTRUM_POWER,SCALING_MODE_LINEAR,DISPLAY_UNIT_VPK,df_F2,win_notch,convertedSpectrum_F2,unit);
			SetCtrlVal(panel, PANEL_2_IDC_FREQ_PEAK_3, freqPeak_F2);
			SetCtrlVal(panel, PANEL_2_IDC_POWER_PEAK_3, powerPeak_F2);
			DeleteGraphPlot (panel, PANEL_2_GRAPH_6, -1, VAL_IMMEDIATE_DRAW);	
	   	 	PlotWaveform(panel, PANEL_2_GRAPH_6, convertedSpectrum_F2, numar_puncte/2 ,VAL_DOUBLE, 1.0, 0.0, 0.0, df_F2,
                                    VAL_FAT_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_RED);
			
			

			break;
	}
	return 0;
}

int CVICALLBACK onTimer (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:

			onSpectrum(freqPanel,PANEL_2_COMMANDBUTTON_3,EVENT_COMMIT,0,0,0);
			//break;
		//case EVENT_KEYPRESS:
			//if (eventData1 ==1280)
				//onSpectrum(freqPanel,PANEL_2_COMMANDBUTTON_3,EVENT_COMMIT,0,0,0);
	}
	return 0;
}

int CVICALLBACK OnSs_2 (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	int img;
	int luna,zi,an,ora,minut,secunda;
	char buffer[256];
	int tip_fereastra;
	char win1[15] = "GaussWin";
	char win2[15] = "RectangleWin";
	char winn[15];
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(panel,PANEL_2_WINDOW_OPTION,&tip_fereastra);
			if(tip_fereastra == 0)
				strcpy(winn,win1);
			else
				strcpy(winn,win2);
			
			
			GetSystemDate(&luna,&zi, &an);
			GetSystemTime(&ora,&minut,&secunda);
			
			GetCtrlDisplayBitmap(panel,PANEL_2_GRAPH_2,1,&img);
			sprintf(buffer, "ss-etapa2\\SG_FILTER_%s-%d-%d-%d_%d-%d-%d.jpg",winn,zi,luna,an,ora,minut,secunda);
			SaveBitmapToJPEGFile(img,buffer,JPEG_DCTFLOAT,100);
			DiscardBitmap(img);
			
			
			GetCtrlDisplayBitmap(panel,PANEL_2_GRAPH_3,1,&img);
			sprintf(buffer, "ss-etapa2\\NOTCH_FILTER_%s-%d-%d-%d_%d-%d-%d.jpg",winn,zi,luna,an,ora,minut,secunda);
			SaveBitmapToJPEGFile(img,buffer,JPEG_DCTFLOAT,100);
			DiscardBitmap(img);
			
			GetCtrlDisplayBitmap(panel,PANEL_2_GRAPH,1,&img);
			sprintf(buffer, "ss-etapa2\\RAW_SIGNAL_%s-%d-%d-%d_%d-%d-%d.jpg",winn,zi,luna,an,ora,minut,secunda);
			SaveBitmapToJPEGFile(img,buffer,JPEG_DCTFLOAT,100);
			DiscardBitmap(img);
			
			GetCtrlDisplayBitmap(panel,PANEL_2_GRAPH_4,1,&img);
			sprintf(buffer, "ss-etapa2\\SPECTRUM_%s-%d-%d-%d_%d-%d-%d.jpg",winn,zi,luna,an,ora,minut,secunda);
			SaveBitmapToJPEGFile(img,buffer,JPEG_DCTFLOAT,100);
			DiscardBitmap(img);
			
			GetCtrlDisplayBitmap(panel,PANEL_2_GRAPH_5,1,&img);
			sprintf(buffer, "ss-etapa2\\SPECTRUM_Filt_SG_%s-%d-%d-%d_%d-%d-%d.jpg",winn,zi,luna,an,ora,minut,secunda);
			SaveBitmapToJPEGFile(img,buffer,JPEG_DCTFLOAT,100);
			DiscardBitmap(img);
			
			GetCtrlDisplayBitmap(panel,PANEL_2_GRAPH_6,1,&img);
			sprintf(buffer, "ss-etapa2\\SPECTRUM_Filt_NOTCH_%s-%d-%d-%d_%d-%d-%d.jpg",winn,zi,luna,an,ora,minut,secunda);
			SaveBitmapToJPEGFile(img,buffer,JPEG_DCTFLOAT,100);
			DiscardBitmap(img);
			

			break;
	}
	return 0;
}
