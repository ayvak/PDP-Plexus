import scipy.misc
from skimage.transform import resize
#import matplotlib
from tensorflow.keras.models import load_model
import matplotlib.pyplot as plt
import numpy as np
import gradio as gr
from sklearn import preprocessing
from tensorflow.keras import Model
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
    
def hidden_layer(hidden,hidden_value):
    #Last layer
    hidden[:12,:4]=1 #column
    hidden[:4,:12]=1 #row
    
    #Second last layer
    hidden[4:16,4:8]=2 #column
    hidden[4:8,4:16]=2 #row
    
    #middle layer
    hidden[8:20,8:12]=3 #column
    hidden[8:12,8:20]=3 #row
    
    #second layer
    hidden[12:24,12:16]=4 #column
    hidden[12:16,12:24]=4 #row
    
    #first layer
    _max = np.max(hidden_value)
    hidden[17:,17:]=hidden_value * (255 / _max) # changing to (0,255) range
    return hidden
    
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
    image = image.reshape(1,28,28,1)
    prediction = model.predict(image).tolist()[0]
    
    #Hidden
    hidden = np.array([[0 for col in range(28)] for row in range(28)])
    layer_name = 'conv2d_1'
    intermediate_layer_model = Model(inputs=model.input,
                                       outputs=model.get_layer(layer_name).output)
    intermediate_output = intermediate_layer_model(image)
    
    intermediate_output=np.squeeze(intermediate_output)
    hidden_value=intermediate_output[:,:,0]
    hidden_layer(hidden,hidden_value)
    
    #Output 
    output = np.array([[0 for col in range(28)] for row in range(28)])
    max_values = sorted(zip(prediction,range(10)),reverse=True)[:3]
    probable_number(output,max_values,0,1) #colour for most probable number
    probable_number(output,max_values,1,1) #colour for second most probable number
    probable_number(output,max_values,2,1) #colour for third most probable number
    
    #Hidden
    hidden = np.array([[0 for col in range(28)] for row in range(28)])
    
    #output is our required matrix
    return {str(i): prediction[i] for i in range(10)}

sketchpad = gr.inputs.Sketchpad()
label = gr.outputs.Label(num_top_classes=3)
interface = gr.Interface(classify, sketchpad, label, live=True,interpretation="default", capture_session=True)

interface.launch(share=True)
