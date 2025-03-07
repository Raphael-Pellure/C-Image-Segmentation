README

KMEANS: <nom_du_programme> <fichier_image> <k> <mode> <fichier_image_reference>  avec mode = perso ou OPCV

exemples: 

1) Perso: ./bin/kmeans ../data/images/texture3.png 2 perso  ../data/images/texture3_VT.png            vs      openCV: ./bin/kmeans ../data/images/texture3.png 2 perso  ../data/images/texture3_VT.png   

2) Perso: ./bin/kmeans ../data/images/texture8.png 2 perso  ../data/images/texture8_VT.png            vs      openCV: ./bin/kmeans ../data/images/texture8.png 2 perso  ../data/images/texture8_VT.png  

2) Perso: ./bin/kmeans ../data/images/texture11.png 2 perso  ../data/images/texture11_VT.png          vs      openCV: ./bin/kmeans ../data/images/texture11.png 2 perso  ../data/images/texture11_VT.png   


MEANSHIFT: <nom_du_programme> <fichier_image> <hs> <hc> <epsilon> <kmax> <fichier_image_reference>

exemples: 
1) Texture 3: ./bin/meanshift  ../data/images/texture3.png 10 20  1e-4 8 ../data/images/texture3_VT.png

2) Texture 8: ./bin/meanshift  ../data/images/texture8.png 10 20  1e-4 8 ../data/images/texture8_VT.png

3) Texture 11: ./bin/meanshift  ../data/images/texture11.png 10 20  1e-4 8 ../data/images/texture11_VT.png

/home/rpe0178/tpTI-v2024.0.0-rc1/src/tp/README.sh