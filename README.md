# Fingerprint_in_STC89C52
实现指纹录入和验证功能，用于学生宿舍指纹改装锁。
主要材料：1.普中科技51单片机开发板，2.指昂科技ZAZ-050指纹模块，3.5-12DC开关，4.12V汽车雨刮器电机，5.5V，12V直流电源，6.线若干。
硬件改造：1.更换主频晶振，将12M更换为11.0592M（12M无法实现9600波特率）；2.拆掉LED点阵（a.lock_switch（）不能输出5V信号，b.太亮，晃眼）
注：寝室是木门
![image](https://github.com/aloneLuhui/FingerprintSTC89C52/blob/master/image/IMG_20181220_134509_1546110718726.jpg)
![image](https://github.com/aloneLuhui/FingerprintSTC89C52/blob/master/image/IMG_20181226_215955.jpg)
![image](https://github.com/aloneLuhui/FingerprintSTC89C52/blob/master/image/VID_20181228_120744.mp4)
