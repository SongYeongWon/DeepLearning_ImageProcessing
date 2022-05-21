"""## Import OpenCV Library"""
from audioop import bias
import math
from operator import imod
from turtle import right, st
import numpy as np
import cv2 as cv
from matplotlib import pyplot as plt
import timeit

cap = cv.VideoCapture("road_straight_student.mp4")

width = cap.get(cv.CAP_PROP_FRAME_WIDTH)
height = cap.get(cv.CAP_PROP_FRAME_HEIGHT)

#------------직선의 기울기, 좌표 변수-----------
m1 = 0          #매번 확인하는 기울기 값
y_intercept = 0 #직선의 y 절편 값

left_val =0     #왼쪽 line y절편 평균 값
left_avg = 0    #왼쪽 line 기울기 평균 값

right_val = 0   #오른쪽 line y절편 평균 값
right_avg = 0   #오른쪽 line 기울기 평균 값

x_left  = 0     #왼쪽 line x 좌표
x_right = 0     #오른쪽 line x 좌표

x_cross = 0     #교점 x 좌표
y_cross = 0     #교점 y 좌표
#------------이전 값 저장 변수들---------------
prev_left_y = 0     #왼쪽 line 이전 y절편 평균 값 저장
prev_left_m = 0     #왼쪽 line 이전 기울기 평균 값 저장

prev_right_y = 0    #오른쪽 line 이전 y절편 평균 값 저장
prev_right_m = 0    #오른쪽 line 이전 기울기 평균 값 저장

#-------------------색상 변수-----------------
left_color = (0,0,0)    #왼쪽 line 색상 초기화
right_color = (0,0,0)   #오른쪽 line 색상 초기화

line_detect = (255,0,0)         #Line detect 경우 Blue
line_non_detect = (0,255,255)   #Line non detect 경우 Yellow
line_change = (0,0,255)         #Line change 경우 Red

In_line_color = (0,255,0)       # 초기 Text green
Line_safe = (0,255,0)           # Safe 경우 green
Line_Dan = (0,0,255)            # Dangerous 경우 red

#-----------------Bias 관련 변수---------------
Bias = 0                #Bias initialization
Bias_Threshold = 20     #Bias Threshold
length_left = 0         #frame 중앙과 왼쪽 line 사이 거리
length_right = 0        #frame 중앙과 오른쪽 line 사이 거리
Threshold_length = 230  # Line과 frame 중앙사이 Threshol 거리 값
Thres_flag = 0          # 0 : 직진, 1 : 우측으로 차선변경, 2 : 좌측으로 차선변경, 3 : frame 중앙에 line 위치
Line_arrow = 0          # Line direction flag (0 : straight, 1 : right, 2 : left)
Line_df = 0

#------------Frame error check Variables-------
frame_num = 0;          #Count frame number 
L_frame = 0             #Left line detect check
R_frame = 0             #Right line detect check
frame_Theshold = 5      #Error Threshold value
No_line_sign = 0        #Frame error Flag ( 0 : safe, 1 : Dangerous)

font =  cv.FONT_HERSHEY_PLAIN

while(1):
    ret, frame = cap.read()
    if frame is None:
        break
    if ret is True: 
        # 알고리즘 시작 시점
        start_t = timeit.default_timer()    #frame 시작 시점 시간
        # start_t = cv.getTickCount()

    '''1. Image pre-processing'''
    #frame 마다 gray scale 로 변환
    frame_gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    #Guassian Filter 적용
    frame_gray = cv.GaussianBlur(frame_gray,(5,5),0)
    # Canny Edge Detection
    dst = cv.Canny(frame_gray, 50, 200, None, 3)

    '''Making ROI process'''
    mask = np.zeros_like(frame_gray)
    vertices = np.array([[100,630],[617,420],[685,420],[1000,630]], dtype=np.int32)
    cv.fillPoly(mask, [vertices], 255)
    #Canny Edege detection 한 이미지와 masking
    ROI_image = cv.bitwise_and(dst, mask)

    '''2. Line detection process'''
    #HoughLineP line 추출
    linesP = cv.HoughLinesP(ROI_image, 1, np.pi / 180 , 65, None, minLineLength=10, maxLineGap=50)
    cv.line(frame, (640,720), (640,630), (255,0,255), 3, cv.LINE_AA)

    #y 절편, 기울기 값 초기화 
    left_m = []
    left_y = []
    right_m = []
    right_y = []

    if linesP is not None:
        for i in range(0, len(linesP)):
            l = linesP[i][0]
            if(l[2]-l[0] != 0):
                m1 = (l[3]-l[1])/(l[2]-l[0])
                y_intercept =  l[1] - m1*l[0] 

            if(Thres_flag==0):   #직진의 경우 left, right line 구분 
                if(m1<-0.55):
                    left_m.append(m1)
                    left_y.append(y_intercept) 
                if(0.5<m1):
                    right_m.append(m1)
                    right_y.append(y_intercept)
            else :              #line 변경 경우 left, right line 구분 
                if(m1<-0.1):
                    left_m.append(m1)
                    left_y.append(y_intercept) 
                if(0<m1):
                    right_m.append(m1)
                    right_y.append(y_intercept)
        
    if( len(right_m) > 0):
        right_avg = sum(right_m)/len(right_m)   # right line y절편 평균값
        right_val = sum(right_y)/len(right_y)   # right line 기울기 평균값
        # right line의 이전 y절편, 기울기 평균 값 저장
        prev_right_y = right_val
        prev_right_m = right_avg
        # right line detection color(blue)
        right_color = line_detect
        # Detected right line count up
        R_frame += 1

    if( len(left_m) > 0):
        left_avg = sum(left_m)/len(left_m)  # left line y절편 평균값
        left_val = sum(left_y)/len(left_y)  # left line 기울기 평균값
        # left line의 이전 y절편, 기울기 평균 값 저장
        prev_left_y = left_val
        prev_left_m = left_avg
        # left line detection color(blue)
        left_color = line_detect
        # Detected left line count up
        L_frame += 1

    if( len(right_m) == 0):
        right_avg = prev_right_m    #이전 right line 의 기울기 값 사용
        right_val = prev_right_y    #이전 right line 의 y절편 값 사용
        # right line non detection color(green)
        right_color = line_non_detect

    if( len(left_m) == 0):
        left_avg = prev_left_m      #이전 left line 의 기울기 값 사용
        left_val = prev_left_y      #이전 left line 의 y절편 값 사용
        # left line non detection color(green)
        left_color = line_non_detect

    ''' Calculation of Line coordinate'''
    x_left = (720-left_val)/left_avg
    x_right = (720-right_val)/right_avg
    x_cross = (right_val-left_val)/(left_avg-right_avg)
    y_cross = left_avg*x_cross + left_val
    length_right = float(round(x_right-640))
    length_left = float(round(640- x_left))

    center = ( (int(x_right) + int(x_left) ) /2 )
    Bias =  float((640-int(center))  / (int(x_right) - int(x_left)) *100)

    img = cv.putText(frame, "Bias  = ", (34, 50), font, 2, (0, 0, 0), 1, cv.LINE_AA)
    if(Bias>0):
        img = cv.putText(frame, "Right", (200, 50), font, 2, (0, 0, 0), 1, cv.LINE_AA)
    elif(Bias<-0):
        img = cv.putText(frame, "Left", (200, 50), font, 2, (0, 0, 0), 1, cv.LINE_AA)
    '''3. Line detection Error check process'''
    #frame error check
    frame_num += 1
    if(frame_num == 20):
        if(L_frame <frame_Theshold and R_frame < frame_Theshold):
            No_line_sign = 1    #두 라인 모두 20 frame 중 5번 미만으로 detect 되는 경우(25%이하) 
        else:
            No_line_sign= 0
        #frame count value initialization
        frame_num = 0
        R_frame=0
        L_frame=0

    '''4. Line Case classification process'''
    #직선 경우 Bias Threshold 값에 따라 구분
    if(-1*Bias_Threshold<Bias<Bias_Threshold):
        Thres_flag = 0
    #우측으로 차선 변경 (Bias Threshold보다 클 경우)
    if(Bias_Threshold<Bias):
        Thres_flag = 1
    #좌측으로 차선 변경 (-Bias Threshold보다 작을 경우)
    if(Bias<-1*Bias_Threshold):
        Thres_flag = 2
    #우측 좌측 라인의 너비가 모두 Threshold_length 보다 작은 경우
    elif((length_right <Threshold_length) and (length_left <Threshold_length) ):
        Thres_flag = 3

    #Lane Class에 따라 line 그리기
    if(Thres_flag==0):
        cv.line(frame, (int(x_left),720), (int(x_cross),int(y_cross)), left_color, 3, cv.LINE_AA)
        cv.line(frame, (int(x_right),720), (int(x_cross),int(y_cross)), right_color, 3, cv.LINE_AA)
    if(Thres_flag == 1):
        cv.line(frame, (int(x_right),720), (int(x_cross),int(y_cross)), line_change, 3, cv.LINE_AA)
    if(Thres_flag == 2):
        cv.line(frame, (int(x_left),720), (int(x_cross),int(y_cross)), line_change, 3, cv.LINE_AA)     
    if(Thres_flag == 3):
        #오른쪽 라인만 detection 되는 경우
        if(len(right_m)>0 and len(left_m) == 0):
            cv.line(frame, (int(x_right),720), (int(x_cross),int(y_cross)), line_change, 3, cv.LINE_AA)
        #오른쪽, 왼쪽 라인 둘다 detection 되는 경우
        elif(len(right_m) > 0 and len(left_m) > 0):
            cv.line(frame, ( int(  (int(x_left) + int(x_right) )/2) ,720), (int(x_cross),int(y_cross)), line_change, 3, cv.LINE_AA)
        #왼쪽 라인만 detection 되는 경우
        else:
            cv.line(frame, (int(x_left),720), (int(x_cross),int(y_cross)), line_change, 3, cv.LINE_AA)       
    cv.line(frame, (int(x_cross),int(y_cross)), (int(x_cross),int(y_cross)), (0,255,255), 10, cv.LINE_AA)

    '''5. Fill the Lane Area'''
    # 추출 영역 색 채우기
    mask_region = np.zeros_like(frame)
    vertices_line = np.array([[int(x_left),720],[int(x_cross),int(y_cross)],[int(x_right),720]], dtype=np.int32)
    img = cv.putText(frame, "In Line  = ", (34,100), font, 2, In_line_color, 1, cv.LINE_AA)
    if(No_line_sign == 0):
        if(Thres_flag != 0):
            In_line_color = Line_Dan
            img = cv.putText(frame, "Line Change", (200,100), font, 2, Line_Dan, 1, cv.LINE_AA)
            cv.fillPoly(mask_region, [vertices_line], (0,0,255))
        else:
            In_line_color = Line_safe
            img = cv.putText(frame, "Safe", (200,100), font, 2, Line_safe, 1, cv.LINE_AA)
            cv.fillPoly(mask_region, [vertices_line], (0,255,0))
    else :
        In_line_color = Line_Dan
        img = cv.putText(frame, "Danger", (200,100), font, 2, Line_Dan, 1, cv.LINE_AA)
        cv.fillPoly(mask_region, [vertices_line], (0,0,255))
    
    #Lane 이동 방향 화살표 그리기
    if Thres_flag == 0:
        prev_bias = round((Bias),2) #Lane 이동 전까지 Bias 값 저장
        img = cv.putText(frame, str(round((abs(Bias)),2)) + '%', (350, 50), font, 2, (0, 0, 0), 1, cv.LINE_AA)
        cv.line(frame, (int(center),720), (int(center),630), (255,255,0), 3, cv.LINE_AA)
    else :
        img = cv.putText(frame, str(abs(prev_bias))+'%', (350, 50), font, 2, (0, 0, 0), 1, cv.LINE_AA)
        if 0< prev_bias < Bias_Threshold - 1 :
            cv.arrowedLine(frame, (int(width/2),int(height/2)), (800, int(height/2)), (0,0,255), 3)
        elif prev_bias < 0 :
            cv.arrowedLine(frame, (int(width/2),int(height/2)), (480, int(height/2)), (0,0,255), 3)

    k = cv.waitKey(5) & 0xFF
    if k == 27:
        break

    # 알고리즘 종료 시점/ FPS 출력
    terminate_t = timeit.default_timer()    
    FPS = int(1./(terminate_t - start_t ))
    img = cv.putText(frame, "FPS  = ", (34, 150), font, 2, (0, 0, 0), 1, cv.LINE_AA)
    img = cv.putText(frame, str(FPS), (200, 150), font, 2, (0, 0, 0), 1, cv.LINE_AA)

    # addWeighted() 함수로 투명도 조절
    weighted_frame = cv.addWeighted(frame, 1, mask_region, 0.3, 0) 
    cv.imshow('weighted_frame',weighted_frame)
cv.destroyAllWindows()
cap.release()