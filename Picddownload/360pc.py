# -*- coding: utf-8 -*-
"""
Created on Sat May 06 20:58:12 2017
download picture from baidu
@author: oempc
"""

import urllib2,re
import requests
from PIL import Image
import os
class BDTP:
    baseurl="http://image.so.com/i?src=rel&q=戴眼镜的人&t=s&zoom=1&color="
    def getPage(self):
        url=self.baseurl
        request=urllib2.Request(url)
        response=urllib2.urlopen(request).read()
        '''
        
        '''
        #print type(response)
        print "response is: "+response
        return response
    
    def down_img(self,url,index):
        web_data=requests.get(url)
        targetfile='E:/Picture/Glass/2/{}.jpg'.format(index)
        with open(targetfile,'wb') as f:
            f.write(web_data.content)
        
    def getImg(self,response):
        #reg = r'http:.+\.jpg'
        reg = r'"thumb":"(.+?\.jpg)"'
        imgre = re.compile(reg)
        imglist = re.findall(imgre,response)
        print len(imglist)
        x = 100
        for imgurl in imglist:
           print str(x)+":"+imgurl.replace('\/','/')+'\r\n'
           try:
               self.down_img(imgurl.replace('\/','/'),50000+x)#必须转义字符处理
           except requests.exceptions.ConnectionError:
               print "error"
               continue
           x += 1
        print x
'''
        reg = r'"objURL":"(.*?)"'
        imgre = re.compile(reg)
        imglist = re.findall(imgre,response)
        x = 120
        for imgurl in imglist:
           print imgurl
           self.down_img(imgurl,x)
           x += 1
'''   
       
          
def renameImg(path,flag=0):
    for parent,dirnames,filenames in os.walk(path):    #三个参数：分别返回1.父目录 2.所有文件夹名字（不含路径） 3.所有文件名字
        x=62;
        if not flag:
            for p in filenames: 
                newname=os.path.join(parent,str(50000+x)+'.jpg')
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
                newname=os.path.join(parent,'new'+str(p)+'.jpg')
                os.rename(os.path.join(parent,p),newname)
                x+=1
'''
            img=Image.open(os.path.join(parent,filenames))
            img=img.resize(256,256)
            img.save(os.path.join(parent,filenames)+'.jpg')
'''
            
x=0
if not x:
   renameImg("E:/Picture/Glass/2/",0)
elif x:       
    b =BDTP()
    res=b.getPage()
    b.getImg(res)

