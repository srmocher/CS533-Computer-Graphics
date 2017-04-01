Instructions:
For the interesting scene, I have created a new scene file interestingScene.txt.
It has 4 objects - batman, bat, chair and a dog with different modeling transformations. It also has 3 light sources.
Simple object - sphere
Moderately complex object - teapot
Complex object - Millennium Falcon


6. The viewing matrix is computed by

w = -g/|g|
u = t x w
v = w x u

M_cam is 
[0 0 -1 0
 0 1  0 0
 1 0  0 -5
 0 0  0  1]


7. P = (3,0,0)
[P' 1]' = M_cam * [P 1]'

 	= [0 0 -1 0         3
	   0 1  0 0         0  
	   1 0  0 -5	*   0	
	   0 0  0  1]       1
	
	= 0
	  0
          -2
	  1

Therefore, transformed point P' = (0,0,-2)


8. Computation of surface normal
The points are A(0,0,0),B(1,0,0) and C(0,1,0)
CA = (0,0,0)-(0,1,0) = (0,-1,0)
AB = (1,0,0) - (0,0,0) = (1,0,0)
Face normal = CA x AB = (0,-1,0) x (1,0,0)
Hence, face normal = (0,0,1) 

