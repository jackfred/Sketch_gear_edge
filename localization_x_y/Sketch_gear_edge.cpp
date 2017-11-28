// localization_x_y.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Socket_client.h"
#include <visp3/gui/vpDisplayOpenCV.h>
#include <visp3/gui/vpDisplayGDI.h>
#include <visp/vpImage.h>
#include <visp/vpDisplay.h>
#include <visp/vpDisplayX.h>
#include <visp/vpMath.h>
#include <visp/vpHomogeneousMatrix.h>
#include <visp/vpFeaturePoint.h>
#include <visp/vpPoint.h>
#include <visp/vpFeatureBuilder.h>
#include <visp/vpException.h>
#include <visp/vpMatrixException.h>
#include <visp/vpServoDisplay.h>
#include <visp/vpDot2.h>
#include <visp/vpPose.h>
#include <visp/vpAdaptiveGain.h>
#include <string>
#include <stdio.h>  
#include <iostream>    //cout
#include <stdio.h> //printf
using namespace cv;
using namespace std;

float center_u = 345;
float center_v = 232;
float height = 0.43;
float u_v_ratio = 0.98;

int _tmain(int argc, _TCHAR* argv[])
{
	Socket_client socket;
	//m means message, p means move to point
	char m_recieve[200];
	char m_ptpspeed[200];
	char m_p[200];
	char m_simulator[200];
	char m_getmotion[200] = "NETS_GETRUNSTATUS 192.168.100.2\0";
	char m_check[200];
	char m_abort[200] = "ABORT";
	//Initialize all memory to 0
	memset(m_recieve, 0, 200);
	//Read return message from robot. Successful connection would get a message : IRA.
	socket.readMessage(socket.sConnect, m_recieve, 200);
	//Set line speed (mm/s)
	memset(m_ptpspeed, 0, 200);
	memset(m_recieve, 0, 200);
	sprintf_s(m_ptpspeed, sizeof(m_ptpspeed), "%s", "SETSPEEDRATE 2000");
	socket.Message(socket.sConnect, m_ptpspeed, m_recieve, 200, false);
	memset(m_ptpspeed, 0, 200);
	memset(m_recieve, 0, 200);
	sprintf_s(m_ptpspeed, sizeof(m_ptpspeed), "%s", "SETACCTIME 25");
	socket.Message(socket.sConnect, m_ptpspeed, m_recieve, 200, false);
	//Set simulator display ratio
	memset(m_simulator, 0, 200);
	memset(m_recieve, 0, 200);
	sprintf_s(m_simulator, sizeof(m_simulator), "%s", "SIMU_ZOOM 0.2 0.2 0.2");
	socket.Message(socket.sConnect, m_simulator, m_recieve, 200, false);
	
	
	//Move to Home
	memset(m_ptpspeed, 0, 200);
	memset(m_recieve, 0, 200);
	sprintf_s(m_ptpspeed, sizeof(m_ptpspeed), "%s", "SETPTPSPEED 1");
	socket.Message(socket.sConnect, m_ptpspeed, m_recieve, 200, false);
	memset(m_p, 0, 200);
	memset(m_recieve, 0, 200);
	sprintf_s(m_p, sizeof(m_p), "%s", "MOVP 0 306.43 102.74 90 0 180");
	socket.Message(socket.sConnect, m_p, m_recieve, 200, false);
	Sleep(200);

	while (1){
		memset(m_check, 0, 200);
		socket.Message(socket.sConnect, m_getmotion, m_check, 200, false);
		Sleep(10);
		if (atoi(m_check) == 0)
		{
			break;
		}
	}

	//Open Camera
	vpImage<unsigned char> I;
	vpImage<unsigned char> I_real_time;
	cv::VideoCapture g(0); // open the default camera
	g.set(CV_CAP_PROP_AUTOFOCUS, 0);
	g.set(CV_CAP_PROP_FOCUS, 60);
	if (!g.isOpened()) { // check if we succeeded
		std::cout << "Failed to open the camera" << std::endl;
		return -1;
	}
	cv::Mat frame;
	cv::Mat frame_real_time;
	int i = 0;
	while ((i++ < 100) && !g.read(frame)) {}; // warm up camera by skiping unread frames
	g >> frame; // get a new frame from camera
	vpImageConvert::convert(frame, I);

	vpDisplayGDI d(I, 0, 0, "Camera view");

	vpDisplay::display(I);
	vpImagePoint center;
	center.set_u(center_u);
	center.set_v(center_v);
	vpDisplay::displayCross(I, center, 3000, vpColor::red);
	vpDisplay::flush(I);

	g >> frame_real_time;
	vpImageConvert::convert(frame_real_time, I_real_time);
	vpDisplayGDI d2(I_real_time, 700, 0, "Camera view real time");
	vpDisplay::display(I_real_time);
	vpDisplay::displayCross(I_real_time, center, 3000, vpColor::red);
	vpDisplay::flush(I_real_time);

	while (1){
		memset(m_ptpspeed, 0, 200);
		memset(m_recieve, 0, 200);
		sprintf_s(m_ptpspeed, sizeof(m_ptpspeed), "%s", "SETPTPSPEED 1");
		socket.Message(socket.sConnect, m_ptpspeed, m_recieve, 200, false);
		memset(m_ptpspeed, 0, 200);
		memset(m_recieve, 0, 200);
		sprintf_s(m_ptpspeed, sizeof(m_ptpspeed), "%s", "SETLINESPEED 5");
		socket.Message(socket.sConnect, m_ptpspeed, m_recieve, 200, false);


		g >> frame_real_time;
		vpImageConvert::convert(frame_real_time, I_real_time);
		vpDisplay::display(I_real_time);
		vpDisplay::displayCross(I_real_time, center, 3000, vpColor::red);
		vpDisplay::flush(I_real_time);

		memset(m_p, 0, 200);
		memset(m_recieve, 0, 200);
		sprintf_s(m_p, sizeof(m_p), "%s", "MOVP 0 306.43 102.74 90 0 180");
		socket.Message(socket.sConnect, m_p, m_recieve, 200, false);
		Sleep(300);

		while (1){
			g >> frame_real_time;
			vpImageConvert::convert(frame_real_time, I_real_time);
			vpDisplay::display(I_real_time);
			vpDisplay::displayCross(I_real_time, center, 3000, vpColor::red);
			vpDisplay::flush(I_real_time);
			memset(m_check, 0, 200);
			socket.Message(socket.sConnect, m_getmotion, m_check, 200);
			Sleep(10);
			if (atoi(m_check) == 0)
			{
				time_t t_start = time(NULL);
				//Keep flushing aquisition						
				while (1){
					time_t t_end = time(NULL);
					g >> frame_real_time;
					vpImageConvert::convert(frame_real_time, I_real_time);
					vpDisplay::display(I_real_time);
					vpDisplay::displayCross(I_real_time, center, 3000, vpColor::red);
					vpDisplay::flush(I_real_time);
					if ((t_end - t_start) > 2) break;
				}
				break;
			}
		}
		


		vpDot2 dot[4];
		vpImagePoint cog;

		std::cout << "Click on the 4 dots clockwise starting from upper/left dot..."
			<< std::endl;

		for (int i = 0; i < 1; i++) {
			dot[i].initTracking(I);
			cog = dot[i].getCog();
			vpDisplay::displayCross(I, cog, 3000, vpColor::blue);
			vpDisplay::flush(I);

		}

		vpCameraParameters cam(609.3, 610, center_u, center_v);
		double x, y;
		vpPixelMeterConversion::convertPoint(cam, dot[0].getCog(), x, y);
		

		double Xc, Yc, Zc;
		Zc = height;
		Xc = x * Zc;
		Yc = y * Zc / u_v_ratio;
		
		vpHomogeneousMatrix cMo;
		vpTranslationVector cto(Xc, Yc, Zc);
		vpRxyzVector cro(vpMath::rad(0), vpMath::rad(0), vpMath::rad(0)); // No rotations
		vpRotationMatrix cRo(cro);
		cMo.buildFrom(cto, cRo); // Build the homogeneous matrix


		vpHomogeneousMatrix M;
		vpTranslationVector t(0, 0, 0);
		vpRxyzVector r(vpMath::rad(0), vpMath::rad(0), vpMath::rad(89.14)); // No rotations
		vpRotationMatrix R(r);
		M.buildFrom(t, R); // Build the homogeneous matrix


		vpHomogeneousMatrix rMc;
		vpTranslationVector rtc(0, 0.30643, 0.10274);
		vpRzyxVector rrc(vpMath::rad(90), vpMath::rad(180), vpMath::rad(0)); // No rotations
		vpRotationMatrix rRo(rrc); // Build the rotation matrix
		rMc.buildFrom(rtc, rRo); // Build the homogeneous matrix

		vpHomogeneousMatrix rMo;
		rMo = rMc*M*cMo;
		//rMo = rMo.inverse();

		//std::cout << rMo.getTranslationVector().data[0] * 1000 << endl << rMo.getTranslationVector().data[1] * 1000 << endl << rMo.getTranslationVector().data[2] * 1000 << endl;

		char c_x[50];
		char c_y[50];
		char c_z[50];
		char c_rx[50];
		char c_ry[50];
		char c_rz[50];
		char message_send_test[200] = "MOVL ";
		char space[2] = " ";
		sprintf_s(c_x, sizeof(c_x), "%f", rMo.getTranslationVector().data[0] * 1000);
		sprintf_s(c_y, sizeof(c_y), "%f", rMo.getTranslationVector().data[1] * 1000);
		sprintf_s(c_z, sizeof(c_z), "%f", -250.0);
		sprintf_s(c_rx, sizeof(c_rx), "%s", "#");
		sprintf_s(c_ry, sizeof(c_ry), "%s", "#");
		sprintf_s(c_rz, sizeof(c_rz), "%s", "#");

		strcat_s(message_send_test, c_x);
		strcat_s(message_send_test, space);
		strcat_s(message_send_test, c_y);
		strcat_s(message_send_test, space);
		strcat_s(message_send_test, c_z);
		strcat_s(message_send_test, space);
		strcat_s(message_send_test, c_rx);
		strcat_s(message_send_test, space);
		strcat_s(message_send_test, c_ry);
		strcat_s(message_send_test, space);
		strcat_s(message_send_test, c_rz);
		strcat_s(message_send_test, space);

		char message_check[200];
		memset(message_check, 0, 200);
		socket.Message(socket.sConnect, message_send_test, message_check, 200);
		Sleep(500);

		while (1){
			g >> frame_real_time;
			vpImageConvert::convert(frame_real_time, I_real_time);
			vpDisplay::display(I_real_time);
			vpDisplay::displayCross(I_real_time, center, 3000, vpColor::red);
			dot[0].setGraphics(true);
			dot[0].setGraphicsThickness(3);
			dot[0].track(I_real_time);
			vpDisplay::flush(I_real_time);
			memset(m_check, 0, 200);
			socket.Message(socket.sConnect, m_getmotion, m_check, 200);
			Sleep(10);
			if (atoi(m_check) == 0)
			{
				time_t t_start = time(NULL);
				//Keep flushing aquisition						
				while (1){
					time_t t_end = time(NULL);
					g >> frame_real_time;
					vpImageConvert::convert(frame_real_time, I_real_time);
					vpDisplay::display(I_real_time);
					vpDisplay::displayCross(I_real_time, center, 3000, vpColor::red);
					dot[0].track(I_real_time);
					vpDisplay::flush(I_real_time);
					if ((t_end - t_start) > 1) break;
				}
				break;
			}
		}


		char m_p_t[200] = "MOVT ";
		float cog_x;
		float cog_y;
		float alpha = 0.03;
		float err_x, err_y;
		float err_x_pre = 0, err_y_pre = 0;
		float integral_x = 0, integral_y = 0;
		float derivative_x, derivative_y;
		float Kp = 0.04, Ki=0, Kd=0.003;
		float out_x, out_y;
		float bias = 0;
		float second;

		memset(m_ptpspeed, 0, 200);
		memset(m_recieve, 0, 200);
		sprintf_s(m_ptpspeed, sizeof(m_ptpspeed), "%s", "SETPTPSPEED 10");
		socket.Message(socket.sConnect, m_ptpspeed, m_recieve, 200, false);

		clock_t temp = 0;
		bool goal = false;

		while (1)
		{
			/*memset(message_check, 0, 200);
			socket.Message(socket.sConnect, m_abort, message_check, 200, false);*/
			memset(m_p_t, 0, 200);
			sprintf_s(m_p_t, sizeof(m_p_t), "%s", "MOVT ");
			g >> frame_real_time;
			vpImageConvert::convert(frame_real_time, I_real_time);
			vpDisplay::display(I_real_time);
			vpDisplay::displayCross(I_real_time, center, 3000, vpColor::red);
			dot[0].track(I_real_time);
			cog = dot[0].getCog();
			cog_x = cog.get_u()-center_u;
			cog_y = cog.get_v()-center_v;

			err_x = cog_x;
			err_y = cog_y;
			integral_x = integral_x + (err_x * 0.15);
			integral_y = integral_y + (err_y * 0.15);
			derivative_x = (err_x - err_x_pre)/0.15;
			derivative_y = (err_y - err_y_pre) / 0.15;
			out_x = Kp*err_x + Ki*integral_x + Kd*derivative_x + bias;
			out_y = Kp*err_y + Ki*integral_y + Kd*derivative_y + bias;
			err_x_pre = err_x;
			err_y_pre = err_y;


			memset(c_x, 0, 50);		
			sprintf_s(c_x, sizeof(c_x), "%f", out_y);
			memset(c_y, 0, 50);
			sprintf_s(c_y, sizeof(c_y), "%f", -out_x);
			strcat_s(m_p_t, c_x);
			strcat_s(m_p_t, space);
			strcat_s(m_p_t, c_y);
			strcat_s(m_p_t, space);
			memset(message_check, 0, 200);
			clock_t c_start = clock();
			cout << (c_start - temp)/(double)(CLOCKS_PER_SEC/1000)<<" ms" << endl;
			temp = c_start;
			
			socket.Message(socket.sConnect, m_p_t, message_check, 200, false);
			
		
		
			while (1){
				g >> frame_real_time;
				vpImageConvert::convert(frame_real_time, I_real_time);
				vpDisplay::display(I_real_time);
				vpDisplay::displayCross(I_real_time, center, 3000, vpColor::red);
				dot[0].track(I_real_time);
				vpDisplay::flush(I_real_time);
				memset(m_check, 0, 200);
				socket.Message(socket.sConnect, m_getmotion, m_check, 200, false);
				if (atoi(m_check) == 0)
				{
					break;
				}
			}
			
			
			
			
			if (cog_x < 1 && cog_y < 1)
			{ 
				err_x_pre = 0, err_y_pre = 0;
				integral_x = 0, integral_y = 0;
			}
		
			
			if (vpDisplay::getClick(I, false))
			{
				Sleep(1000);
				break;
			}
		}

	}
	return 0;
}

