import cv2
import os
import pandas as pd
import numpy as np
from ultralytics import YOLO
import datetime
import time
import pickle
import torch
import serial
from collections import Counter
import gunshot_detection1 as gs

ser = serial.Serial('/dev/serial0', 115200)
path = 'best.pt'
model = torch.hub.load(r'/home/nalaka/yolov5', 'custom', path , source='local')

# Reading an image in default
cap = cv2.VideoCapture("Elephants.mp4")

def send_notification(data):
    try:
        ser.write(data.encode())
        print(f"Data sent to ESP32: {data}")
    except Exception as e:
        print(f"Error sending data: {str(e)}")

# Adjust the input resolution for faster processing (e.g., 320x240)
width = 320
height = 240
        
# Initialize variables
detection = False
last_detection_time = 0
timestamp = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
#Image Processing and area append 
while True:    
    ret, frame = cap.read()   
  
    img0 = cv2.resize(frame, (width, height))
    
    results=model(img0)
    
    objects = results.names
    a = results.pred[0][:, -1].cpu().numpy().tolist()
    #a=results[0].boxes.data
    px = [objects[int(idx)] for idx in a]
    
    class_counts = Counter(px)
    
    for class_name, count in class_counts.items():
        ready =(f"{class_name}:{count}")
        #print(ready)
    
        if class_name == "Elephant":  # Replace with your actual detection result
            #Elephant_detected = True
            current_time = time.time() # Record the time of detection
            if current_time - last_detection_time >= 30:  # 300 seconds = 5 minutes
                # Data to send to ESP32
                data_to_send = "elephant\n"
                            
                last_detection_time = current_time
                
        elif class_name == "Fire":
            current_time = time.time()
            if current_time - last_detection_time >= 30:  # 300 seconds = 5 minutes
                # Data to send to ESP32
                data_to_send = "fire\n"
                #print(data_to_send)            
                last_detection_time = current_time
        
        elif class_name == "Weapon":
            current_time = time.time()
            if current_time - last_detection_time >= 10:  # 300 seconds = 5 minutes
                # Data to send to ESP32
                data_to_send = "weapon\n"
                #print(data_to_send)            
                last_detection_time = current_time
        
        elif class_name == "Bear":
            current_time = time.time()
            if current_time - last_detection_time >= 10:  # 300 seconds = 5 minutes
                # Data to send to ESP32
                data_to_send = "bear\n"
                #print(data_to_send)            
                last_detection_time = current_time
                
        elif class_name == "Leopard":
            current_time = time.time()
            if current_time - last_detection_time >= 10:  # 300 seconds = 5 minutes
                # Data to send to ESP32
                data_to_send = "leopard\n"            
                last_detection_time = current_time
        else:
            detection = False
        
        gunshot = gs.sound()
        
        print(gunshot)
    
        print(data_to_send)
        send_notification(data_to_send) 
    path = 'images'    
    #output_filename = f'output_{timestamp}.jpg'
    cv2.imwrite(os.path.join(path,f'output_{timestamp}.jpg'), img0)
    cv2.imshow("Path", img0)

    if cv2.waitKey(1)&0xFF==27:
        break
    
ser.close()    
cap.release()
cv2.destroyAllWindows()



