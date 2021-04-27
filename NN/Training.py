from tensorflow.keras.models import Sequential




from tensorflow.keras.datasets import mnist

#import matplotlib.pyplot as plt

from tensorflow.keras.layers import Dense

from tensorflow.keras.layers import Flatten

from tensorflow.keras.utils import to_categorical

import numpy as np





from tensorflow.keras.layers import Dropout

from tensorflow.keras.layers import Convolution2D

from tensorflow.keras.layers import MaxPooling2D

from tensorflow.keras import backend as K

from tensorflow.keras.optimizers import SGD




(trainX, trainY), (testX, testY) = mnist.load_data()

# reshape dataset to have a single channel

trainX = trainX.reshape((trainX.shape[0], 28, 28, 1))

testX = testX.reshape((testX.shape[0], 28, 28, 1))

	# one hot encode target values

trainY = to_categorical(trainY)

testY = to_categorical(testY)




trainX = trainX.astype('float32')

testX = testX.astype('float32')




# normalize to range 0-1

#trainX = trainX / 255.0

#testX = testX / 255.0

trainX[trainX <=0.5] = 0

trainX[trainX >0.5] = 1




testX[testX <=0.5] = 0

testX[testX >0.5] = 1







 #otsu like algos for right threshold




#Seeing the images

#for i in range(1,5): 

#    plt.imshow(x_train[i]) 

#    plt.show() 

num_classes = 10

model = Sequential()

model.add(Convolution2D(32, (3, 3), activation='relu', kernel_initializer='he_uniform', input_shape=(28, 28, 1)))

model.add(MaxPooling2D((2, 2)))

#add one more layer

model.add(Convolution2D(16, (3, 3), activation='relu', kernel_initializer='he_uniform'))

model.add(MaxPooling2D((2, 2)))

model.add(Flatten())

model.add(Dense(100, activation='relu', kernel_initializer='he_uniform'))

model.add(Dense(10, activation='softmax'))

model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])

model.fit(trainX, trainY, validation_data = (testX,testY), epochs = 15, shuffle= True, verbose = 2, batch_size= 128)




#validation test

model.save("PDP_NN")
