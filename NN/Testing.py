#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Feb 22 04:03:50 2021

@author: kavya
"""





import scipy.misc
from skimage.transform import resize
import matplotlib
from keras.models import load_model
import matplotlib.pyplot as plt
import numpy as np
import gradio as gr
from sklearn import preprocessing

model = load_model("PDP_NN")

#printing most probable number
def probable_number(output,max_values,number,col):
    #number = 0 -----> Most Probable
    #number = 1 -----> Second Most Probable
    #number = 2 -----> Third Most Probable
    
    #output - LED matrix
    
    #max_values - list of tuples containing the maximum probable numbers and their probabilities
    
    #col - What colour (number) needs to be put for the respective layer
    if max_values[number][1] == 0:
        output[2 + number*9 ,1:6]=col
        output[8 + number*9 ,1:6]=col
        output[2 + number*9:9 + number*9 ,1]=col
        output[2+ number*9:9+ number*9 ,5]=col

    if max_values[number][1] == 1:
        output[2+ number*9:9+ number*9 ,5]=col

    if max_values[number][1] == 2:   
        output[2+ number*9 ,1:6]=col
        output[5+ number*9 ,1:6]=col
        output[8+ number*9 ,1:6]=col
        output[2+ number*9:5+ number*9 ,5]=col
        output[5+ number*9:9+ number*9 ,1]=col

    if max_values[number][1] == 3:   
        output[2+ number*9 ,1:6]=col
        output[5+ number*9 ,1:6]=col
        output[8+ number*9 ,1:6]=col
        output[2+ number*9:9+ number*9 ,5]=col

    if max_values[number][1] == 4:
        output[5+ number*9,1:6]=col
        output[2+ number*9:9+ number*9,5]=col
        output[2+ number*9:5+ number*9,1]=col

    if max_values[number][1] == 5:   
        output[2+ number*9,1:6]=col
        output[5+ number*9,1:6]=col
        output[8+ number*9,1:6]=col
        output[2+ number*9:5+ number*9,1]=col
        output[5+ number*9:9+ number*9,5]=col

    if max_values[number][1] == 6:   
        output[2+ number*9,1:6]=col
        output[5+ number*9,1:6]=col
        output[8+ number*9,1:6]=col
        output[2+ number*9:9+ number*9,1]=col
        output[5+ number*9:9+ number*9,5]=col

    if max_values[number][1] == 7:  
        output[2+ number*9,1:6]=col
        output[2+ number*9:9+ number*9,5]=col

    if max_values[number][1] == 8:
        output[2+ number*9,1:6]=col
        output[5+ number*9,1:6]=col
        output[8+ number*9,1:6]=col
        output[2+ number*9:9+ number*9,1]=col
        output[2+ number*9:9+ number*9,5]=col

    if max_values[number][1] == 9:
        output[2+ number*9,1:6]=col
        output[5+ number*9,1:6]=col
        output[8+ number*9,1:6]=col
        output[2+ number*9:5+ number*9,1]=col
        output[2+ number*9:9+ number*9,5]=col
        
    #probability of the numbers
    output[4+number*9:7+number*9,8:9+round(max_values[number][0]*18)]=col
    
def classify(image):
    #print(image.shape)
    image = image/255.0
    image=image.reshape(28,28)
    is_all_zero = np.all((image == 0))
    if not is_all_zero:
        x, y = np.nonzero(image)
    # Using the smallest and largest x and y indices of nonzero elements, 
    # we can find the desired rectangular bounds.  
    # And don't forget to add 1 to the top bound to avoid the fencepost problem.
        image= image[x.min():x.max()+1, y.min():y.max()+1]
        result = np.zeros((image.shape[0]+10,image.shape[1]+10))
        result[5:image.shape[0]+5,5:image.shape[1]+5] = image
        image = resize(result, (28, 28))
        matplotlib.image.imsave('outfile.jpg',image)
    image = image.reshape(1,28,28,1)
    prediction = model.predict(image).tolist()[0]
    output = np.array([[0 for col in range(28)] for row in range(28)])
    max_values = sorted(zip(prediction,range(10)),reverse=True)[:3]
    probable_number(output,max_values,0,1) #colour for most probable number
    probable_number(output,max_values,1,1) #colour for second most probable number
    probable_number(output,max_values,2,1) #colour for third most probable number
    
    #output is our required matrix
    return {str(i): prediction[i] for i in range(10)}

sketchpad = gr.inputs.Sketchpad()
label = gr.outputs.Label(num_top_classes=3)
interface = gr.Interface(classify, sketchpad, label, live=True,interpretation="default", capture_session=True)

interface.launch(share=True)