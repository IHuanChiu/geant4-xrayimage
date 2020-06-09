import os,sys,math,ROOT

def getangle(hist_name):
    n_angle_step = 6
    initposition = 0
    if    "h_0" in hist_name: return (-1)*(initposition + 0 )  * 2*(math.pi)/n_angle_step
    elif  "h_1" in hist_name: return (-1)*(initposition + 1 )  * 2*(math.pi)/n_angle_step
    elif  "h_2" in hist_name: return (-1)*(initposition + 2 )  * 2*(math.pi)/n_angle_step
    elif  "h_3" in hist_name: return (-1)*(initposition + 3 )  * 2*(math.pi)/n_angle_step
    elif  "h_4" in hist_name: return (-1)*(initposition + 4 )  * 2*(math.pi)/n_angle_step
    elif  "h_5" in hist_name: return (-1)*(initposition + 5 )  * 2*(math.pi)/n_angle_step
    

