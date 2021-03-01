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
    print(f"prediction : {prediction}")
    return {str(i): prediction[i] for i in range(10)}
sketchpad = gr.inputs.Sketchpad()
label = gr.outputs.Label(num_top_classes=3)
interface = gr.Interface(classify, sketchpad, label, live=True, capture_session=True)

interface.launch(share=True)