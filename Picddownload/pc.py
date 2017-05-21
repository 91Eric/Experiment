# -*- coding: utf-8 -*-
"""
Created on Sat May 06 20:58:12 2017
download picture from baidu
@author: oempc
"""

import urllib2,re
import requests
from PIL import Image
from skimage import io,transform 
import os
import argparse
class BDTP:
    baseurl="https://image.baidu.com/search/flip?tn=baiduimage&ie=utf-8&word=%E5%8D%96%E6%A3%92%E7%90%83%E5%B8%BD&pn=60&gsm=0"
    def __init__(self,url=baseurl):
        self.baseurl = url;
    def getPage(self):
        url=self.baseurl
        request=urllib2.Request(url)
        response=urllib2.urlopen(request).read()
        '''
        
        '''
        #print type(response)
        print response
        return response
    
    def down_img(self,url,index):
        web_data=requests.get(url)
        targetfile='E:\Picture\Hat\{}.jpg'.format(index)
        with open(targetfile,'wb') as f:
            f.write(web_data.content)
        
    def getImg(self,response):
        #reg = r'http:.+\.jpg'
        reg = r'"objURL":"(.+?\.jpg)"'
        imgre = re.compile(reg)
        imglist = re.findall(imgre,response)
        #print type(imglist)
        x = 775
        for imgurl in imglist:
           print str(x)+":"+imgurl+'\r\n'
           try:
               self.down_img(imgurl,10000+x)
           except requests.exceptions.ConnectionError:
               print "error"
               continue
           x += 1
        print x       
          
def renameAllImg(path,flag=0):
    for parent,dirnames,filenames in os.walk(path):    #三个参数：分别返回1.父目录 2.所有文件夹名字（不含路径） 3.所有文件名字
        x=0;
        if not flag:
            for p in filenames: 
                newname=os.path.join(parent,str(20000+x)+'.jpg')
                print p
                os.rename(os.path.join(parent,p),newname)
                x+=1
                #
            '''
            if os.path.exists(newname):
                os.rename(os.path.join(parent,p),newname)
                print "delete"+newname+'done'
            '''
                
        else:
            for p in filenames: 
                newname=os.path.join(path,'new'+str(p)+'.jpg')
                os.rename(os.path.join(path,p),newname)
                x+=1
def renameImg(path):
    filenames = os.listdir(path)  
    x = 0
    for filename in filenames:
        if os.path.isdir(path+filename):
            continue
        else:
            newname=os.path.join(path,str(10000+x)+'.jpg')
            os.rename(os.path.join(path,filename),newname)
            x+=1            
def resizeImage(path):  
    for parent,dirnames,filenames in os.walk(path):
        for filename in filenames:
            img=io.imread(os.path.join(parent,filename))
            print  filename
            img=transform.resize(img,(256,256))
            io.imsave(os.path.join(parent,'0'+filename),img)
                
'''
            
'''
           
x=0
if not x:
   renameAllImg("E:/Picture/Hat/cut/")
elif x:       
    b =BDTP()
    res=b.getPage()
    b.getImg(res)

#以下是用于命令行时的参数
if __name__ == "__main__":
    parser=argparse.ArgumentParser()
    #parser.add_argument('-t')
    group=parser.add_mutually_exclusive_group()
    group.add_argument("-d", '--download', action = 'store_true', help = "download flag")
    group.add_argument('-r', '--rename', action ='store_true', help = 'rename flag')
    group.add_argument('-c', '--changesize', action ='store_true', help = 'changesize flag')
    
    group2=parser.add_argument_group(description='help')
    group2.add_argument('-u', '--url', help = 'url you want download from')
    group2.add_argument('-p', '--path', help = 'path to deal with rename or resize')
    args = parser.parse_args()
    print args
    if args.download:
        if args.url is not None:
            b = BDTP(args.url)
            res = b.getPage()
            b.getImg(res)
        else:
            print 'url need'
    elif args.rename:
        if args.path is None:
            print 'you should privide a path'
        else:
            if os.path.exists(args.path):
                renameImg(args.path)
    elif args.changesize:
        if args.path is None:
            print 'you should privide a path'
        else:
            if os.path.exists(args.path):
                resizeImage(args.path)
    
            
        