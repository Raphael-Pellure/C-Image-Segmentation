#include "ocv_utils.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void printHelp(const string& progName) {
    cout << "Usage:\n\t " << progName << " <image_file> <hs> <hc> <epsilon> <kmax> [<ground_truth_file>]" << endl;
}

// Fonction pour calculer les métriques de segmentation (TP, FP, TN, FN)
void calculdonnees(const Mat& img_mean_shifted, const Mat& groundTruth) {

    double TP = 0, FP = 0, TN = 0, FN = 0;

    for (int i = 0; i < img_mean_shifted.rows; i++) {
        for (int j = 0; j < img_mean_shifted.cols; j++) {
            uchar pixel_meanshifted = img_mean_shifted.at<uchar>(i, j);
            uchar pixelderef = groundTruth.at<uchar>(i, j);


            if (pixel_meanshifted == pixelderef && pixel_meanshifted == 255) TP++; //Vrais positifs
            else if (pixel_meanshifted == pixelderef && pixel_meanshifted == 0) TN++; //Vrais négatifs
            else if (pixel_meanshifted != pixelderef && pixel_meanshifted == 255) FP++; //Faux positifs
            else if (pixel_meanshifted != pixelderef && pixel_meanshifted == 0) FN++; //Faux négatifs
        }
    }

    double P = TP / (TP + FP);                //Précision
    double S = TP / (TP + FN);                //Sensibilité
    double DSC = 2 * TP / (2 * TP + FP + FN); //Coefficient de similarité (DSC)

    printf("-------------------- \n");
        printf("TP = %f \n", TP);
        printf("FP = %f \n", FP);
        printf("TN = %f \n", TN);
        printf("FN = %f \n", FN);
        printf("-------------------- \n");
        printf("La précision est de %f \n", P);
        printf("La sensibilité est de %f \n", S);
        printf("Le coefficient de similarité est de %f \n", DSC);
}

// Fonction pour calculer la nouvelle couleur d'un pixel en fonction de ceux présents dans la fenêtre qui l'entoure 
Vec3b moyenne_fenetre(const Mat& image, int x, int y, float hs, float hc) {

    // hs => fenêtre d'observation autour du pixel étudié de taille (2hs + 1)*(2hs + 1)
    int x_ini = max(0, static_cast<int>(x - hs / 2));             // static_cast: converti en int avant d'appliquer max qui prend des int en arguments
    int x_fin = min(image.rows - 1, static_cast<int>(x + hs / 2));

    int y_ini = max(0, static_cast<int>(y - hs / 2));
    int y_fin = min(image.cols - 1, static_cast<int>(y + hs / 2));

    Vec3d sommeRGB(0, 0, 0); 
    int nb_elt = 0;
    
    for (int a = x_ini; a <= x_fin; a++) {
        for (int b = y_ini; b <= y_fin; b++) {

            if(norm(image.at<Vec3b>(x, y) - image.at<Vec3b>(a,b), NORM_L2) < hc){
            Vec3b pixel = image.at<Vec3b>(a, b);
            sommeRGB[0] = sommeRGB[0] + pixel[0];
            sommeRGB[1] = sommeRGB[1] + pixel[1];
            sommeRGB[2] = sommeRGB[2] + pixel[2];
            nb_elt++;
            }
        }   
    }

    //Si nb_elt > 0 on retourne la moyenne sur chaque canal sinon on garde le pixel étudié
    return (nb_elt > 0) ? Vec3b(sommeRGB[0] / nb_elt, sommeRGB[1] / nb_elt, sommeRGB[2] / nb_elt) : image.at<Vec3b>(x, y);
}

// Fonction qui pour tous les pixels de l'image applique moyenne_fentre tant que la précision epsilon ou kmax n'est pas atteint
void meanshift(Mat& image, float hs, float hc, float epsilon, int kmax) {

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {

            Vec3b moyenne = moyenne_fenetre(image, i, j, hs, hc);
            int k = 0;

            while ((norm(image.at<Vec3b>(i, j) - moyenne, NORM_L2) > epsilon) && (k < kmax)) { // kmax est le nombre max d'itération par pixel et epsilon critère d'
                image.at<Vec3b>(i, j) = moyenne;                // affectation de la moyenne de la fenêtre au pixel parcourus
                moyenne = moyenne_fenetre(image, i, j, hs, hc); // calcul de la nouvelle moyenne
                k++;
            }
        }
    }
}

// Définition de l'image binarisée
Mat binarisation(Mat& image) {

    Mat binarisation = image.clone();
    Vec3d moyenne_img_totale(0, 0, 0);

    double nb_eleme_image = image.rows * image.cols;
    double seuil = 0;

    // Calcul de la moyenne de l'intensité de l'image 
    for (int i = 0; i < binarisation.rows; i++) {
        for (int j = 0; j < binarisation.cols; j++) {
            moyenne_img_totale += binarisation.at<Vec3b>(i, j);
        }
    }
    moyenne_img_totale /= nb_eleme_image;
    seuil = (moyenne_img_totale[0] + moyenne_img_totale[1] + moyenne_img_totale[2]) / 3; //Intensité moyenne = moyenne sur RGB

    printf("-------------------- \n");
    printf("seuil de la binarisation = %f\n", seuil); // pour analyser la méthode de binarisation utilisée

    // Binarisation de l'image
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {

            double somme_pixel_image = binarisation.at<Vec3b>(i, j)[0] + binarisation.at<Vec3b>(i, j)[1] + binarisation.at<Vec3b>(i, j)[2];
            somme_pixel_image /= 3; // Intensité du pixel parcouru

            if (somme_pixel_image < seuil) {
                binarisation.at<Vec3b>(i, j) = Vec3b(0, 0, 0); // On affecte noir si l'intensité de ce pixel est inférieur à l'intensité moyenne de l'image
            } else {
                binarisation.at<Vec3b>(i, j) = Vec3b(255, 255, 255); // Blanc si supérieur
            }
        }
    }
    return binarisation;
}

int main(int argc, char** argv) {
    if (argc < 6 || argc > 7) {
        cout << "Incorrect number of arguments." << endl;
        printHelp(string(argv[0]));
        return EXIT_FAILURE;
    }

    const string imageFilename = string(argv[1]);
    const float hs = stof(argv[2]);
    const float hc = stof(argv[3]);
    const float epsilon = stof(argv[4]);
    const int kmax = stoi(argv[5]);
    const string groundTruthFilename = string(argv[6]);
    

    cout << "Program called with the following arguments:" << endl;
    cout << "\timage file: " << imageFilename << endl;
    cout << "\ths: " << hs << endl;
    cout << "\thc: " << hc << endl;
    cout << "\tepsilon: " << epsilon << endl;
    cout << "\tkmax: " << kmax << endl;
    cout << "\timage file: " << groundTruthFilename << endl;
    

    // Chargement de l'image
    Mat image = imread(imageFilename, IMREAD_COLOR);
    Mat image_reference;

    if (!groundTruthFilename.empty()) {
        image_reference = imread(groundTruthFilename, IMREAD_COLOR);
        //image_reference.convertTo(image_reference, CV_8UC3);
        
    }

    if (image_reference.empty()) {
        cerr << "Error: Could not load the image!" << endl;
        return EXIT_FAILURE;
    }


    if (image.empty()) {
        cerr << "Error: Could not load the image!" << endl;
        return EXIT_FAILURE;
    }

    // Appliquer MeanShift
    Mat img_mean_shifted = image.clone();
    meanshift(img_mean_shifted, hs, hc, epsilon, kmax);

    // Binarisation
    Mat bina = binarisation(img_mean_shifted);

    

    if (!groundTruthFilename.empty()) {
        calculdonnees(bina, image_reference); //erreur de dimension incompréhensible
    }
    
    imshow("Image Originale", image);
    // Display and save
    imshow("Segmented Image by Meanshift", img_mean_shifted);
    imwrite("Segmented_Meanshift.png", img_mean_shifted);


    // Display and save pour la binarisation
    imshow("Segmented Image by Meanshift binarisé", bina);
    imwrite("Segmented_Meanshift_binarise.png", bina);

    imshow("Image de Reference", image_reference);

    waitKey(0);

    destroyAllWindows();


    return EXIT_SUCCESS;
}
