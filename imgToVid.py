from PIL import Image
import numpy as np
import random
import os
import imageio
import subprocess
import math
import time
from mod import Mod
base_dir = os.path.realpath(".")
print(base_dir)
_mainDir=sorted(os.listdir('.'))
print(_mainDir)
img_dir = os.path.join(base_dir,"images");

class Renderer:
     def __init__(self,frameRate):
          self.frameRate = frameRate
          print("board initilizing")

     def renderVideo(self):
          proc = subprocess.Popen(
               [
               "ffmpeg",
               "-r", str(self.frameRate),
               "-f", "image2",
               "-i", os.path.join(base_dir,"images",'%5d.ppn'),
                    os.path.join(base_dir, "renders",f"{time.time()}_gol.mov")
          ]
          )
     def pad(self,number,paddSize):
          charCount=0
          padd=""
          for char in str(number):
               charCount=charCount+1
          if(charCount>paddSize):
               raise Exception("well, got some bad shit here")
          if(charCount==paddSize):
               return number
          else:
               while(charCount<paddSize):
                    padd=padd+"0"
                    charCount=charCount+1
          return padd+str(number)
     def forMateImages(self):

          for count, filename in enumerate(os.listdir(img_dir)):
               dstString = self.pad((count),5) + ".ppn"
               src = os.path.join(img_dir,filename)
               dst = os.path.join(img_dir,dstString)     
               # rename() function will
               # rename all the files
               os.rename(src, dst)
frameRate =16;

R =  Renderer(frameRate)
R.forMateImages()
R.renderVideo()