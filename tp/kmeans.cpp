#include "ocv_utils.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace cv;
using namespace std;

void printHelp(const string& progName)
{
    cout << "Usage:\n\t " << progName << " <image_file> <K_num_of_clusters> [<image_ground_truth>]" << endl;
}

// Fonction pour initialiser les centres des clusters aléatoirement
void initializeCenters(const Mat& imageFloat, vector<Vec3f>& centers, int k)
{
    srand(time(0)); // Initialiser la seed aléatoirement
    for (int i = 0; i < k; ++i) {
        int randomRow = rand() % imageFloat.rows;
        int randomCol = rand() % imageFloat.cols;
        centers[i] = imageFloat.at<Vec3f>(randomRow, randomCol);
    }
}

// Fonction pour affecter les labels en fonction des distances aux centres
void assignLabels(const Mat& imageFloat, const vector<Vec3f>& centers, Mat& labels)
{
    for (int i = 0; i < imageFloat.rows; i++) {
        for (int j = 0; j < imageFloat.cols; j++) {

            Vec3f pixel = imageFloat.at<Vec3f>(i, j);
            double dist_1 = norm(pixel - centers[0], NORM_L2); //norme euclienne
            double dist_2 = norm(pixel - centers[1], NORM_L2);

            if (dist_1 <= dist_2) { //assignement au cluster le plus proche
                labels.at<uchar>(i, j) = 0;
            } else {
                labels.at<uchar>(i, j) = 255;
            }
        }
    }
}

// Fonction pour mettre à jour les centres en prenant la moyenne des pixels assignés à chaque cluster
void updateCenters(const Mat& imageFloat, const Mat& labels, vector<Vec3f>& centers)
{
    Vec3f color_1(0, 0, 0), color_2(0, 0, 0);
    double nb_elt_1 = 0, nb_elt_2 = 0;

    for (int i = 0; i < imageFloat.rows; i++) {
        for (int j = 0; j < imageFloat.cols; j++) {

            Vec3f pixel = imageFloat.at<Vec3f>(i, j);

            if (labels.at<uchar>(i, j) == 0) { //Somme les couleurs du cluster 1
                color_1 += pixel;
                nb_elt_1++;

            } else {                           //Somme les couleurs du cluster 2
                color_2 += pixel;
                nb_elt_2++;
            }
        }
    }

    // Calcul des nouveaux centres
    centers[0] = (nb_elt_1 > 0) ? Vec3f(color_1[0] / nb_elt_1, color_1[1] / nb_elt_1, color_1[2] / nb_elt_1) : centers[0]; //Moyenne en faisant attention au cas nb_elt = 0
    centers[1] = (nb_elt_2 > 0) ? Vec3f(color_2[0] / nb_elt_2, color_2[1] / nb_elt_2, color_2[2] / nb_elt_2) : centers[1];
}

// Fonction pour vérifier si les centres ont convergé
bool hasConverged(const vector<Vec3f>& old_centers, const vector<Vec3f>& new_centers)
{
    return (old_centers[0] == new_centers[0] && old_centers[1] == new_centers[1]);
}

// Fonction pour calculer les métriques de segmentation (TP, FP, TN, FN)
void calculdonnees(const Mat& segmented, const Mat& groundTruth, double& TP, double& FP, double& TN, double& FN)
{
    Mat vector_segmented = segmented.reshape(1, segmented.rows * segmented.cols); //Reshape en vecteur colonne pour comparer plus facilement 
    Mat vector_ref = groundTruth.reshape(1, groundTruth.rows * groundTruth.cols);

    for (int i = 0; i < segmented.rows * segmented.cols; i++) {
        if (vector_segmented.at<uchar>(i) == vector_ref.at<uchar>(i) && vector_segmented.at<uchar>(i) == 255) TP++; //Vrais positifs (Blanc bien détecté)
        else if (vector_segmented.at<uchar>(i) == vector_ref.at<uchar>(i) && vector_segmented.at<uchar>(i) == 0) TN++; //Vrais négatifs (Noir bien détecté)
        else if (vector_segmented.at<uchar>(i) != vector_ref.at<uchar>(i) && vector_segmented.at<uchar>(i) == 255) FP++; //Faux positifs
        else if (vector_segmented.at<uchar>(i) != vector_ref.at<uchar>(i) && vector_segmented.at<uchar>(i) == 0) FN++; //Faux négatifs
    }
    
}

int main(int argc, char** argv)
{
    if (argc != 4 && argc != 5)
    {
        cout << " Incorrect number of arguments." << endl;
        printHelp(string(argv[0]));
        return EXIT_FAILURE;
    }

    const auto imageFilename = string(argv[1]);
    const int k = stoi(argv[2]);
    const string method = (argc = 5) ? string(argv[3]) : string();
    const string groundTruthFilename = (argc == 5) ? string(argv[4]) : string();


    cout << " Program called with the following arguments:" << endl;
    cout << " \timage file: " << imageFilename << endl;
    cout << " \tk: " << k << endl;
    cout << " \tmethod: " << method << endl;
    if(!groundTruthFilename.empty()) cout << " \tground truth segmentation: " << groundTruthFilename << endl;
    printf("-------------------- \n");


    // Chargement de l'image à modifier et de l'image de référence
    Mat image = imread(imageFilename, IMREAD_COLOR);
    Mat imageref = imread(groundTruthFilename, IMREAD_COLOR);

    PRINT_MAT_INFO(image);
    PRINT_MAT_INFO(imageref);

    // Convertion de l'image en float
    Mat imageFloat;
    image.convertTo(imageFloat, CV_32F);

    // Initialisation des centres
    vector<Vec3f> centers(k);
    initializeCenters(imageFloat, centers, k);

    // Initialisation des labels
    Mat labels(image.rows, image.cols, CV_8U);
    Mat old_labels = labels.clone();

    int modif = 1;
    int Nmax = 10; // Nombre max d'iterations
    int N = 0;

    while (modif == 1 && N < Nmax) {
        old_labels = labels.clone();
        
        // 1: Assigner un label à chaque pixel
        assignLabels(imageFloat, centers, labels);
        
        // 2: Calcul des nouveaux centres
        vector<Vec3f> new_centers = centers;
        updateCenters(imageFloat, labels, new_centers);
        
        // 3: Test si l'algo a convergé
        if (hasConverged(centers, new_centers)) {
            modif = 0;
        }

        centers = new_centers; // Assigne les nouveaux centres
        N++;
    }

    // Creation de l'image segmenté en noir est blanc
    Mat segmented(image.rows, image.cols, CV_8U);
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {

            segmented.at<uchar>(i, j) = labels.at<uchar>(i, j);
                                                                             
        }
    }



    ///////////////////////// APPEL DE KMEANS D'OPENCV ///////////////////////////////////////////////////////////////////////

    // 1) in order to call kmeans we need to first convert the image into floats (CV_32F)
    // see the method Mat.convertTo()
    image.convertTo(image,CV_32F);
    
    // 2) kmeans asks for a mono-dimensional list of "points". Our "points" are the pixels of the image that can be seen as 3D points
    // where each coordinate is one of the color channel (e.g. R, G, B). But they are organized as a 2D table, we need
    // to re-arrange them into a single vector.
    // see the method Mat.reshape(), it is similar to matlab's reshape
    Mat vector = image.reshape(1,image.rows*image.cols);
    
    // now we can call kmeans(...)
    Mat labels_OPCV ;
    Mat centers_OPCV ;

    // TermCriteria => critère d'arret avec max 10 iter 
    // 1 => execute l'algo 1 fois pour un labelling différent et garde le meilleur
    // KMEANS_RANDOM_CENTERS = initialisation aléatoire des centres
    kmeans(vector, k, labels_OPCV, TermCriteria(TermCriteria::MAX_ITER, 10, 0), 1, KMEANS_RANDOM_CENTERS, centers_OPCV) ; 

    labels_OPCV = labels_OPCV * 255;

    // Convertion des centres
    centers_OPCV.convertTo(centers_OPCV, CV_8U) ; 
    
    // Création matrice image segmenté
    Mat segmented_OPCV(image.rows * image.cols, 1, CV_8U);
    
    // Remplacement des couleurs par cluster
    for (int i = 0; i < image.rows * image.cols; i++){

        if (labels_OPCV.at<int>(i,0) <= 127){segmented_OPCV.at<uchar>(i) = 0;}
        else{segmented_OPCV.at<uchar>(i) = 255;}   
    }
        
    // Remise en forme      
    segmented_OPCV = segmented_OPCV.reshape(1, image.rows); //arg1 = nbr canaux (rbg=1 ici) , arg2 = nbr ligne après
    

    ///////////////////////////// FIN KMEANS OPENCV ////////////////////////////////////////////////////////////////////////////////////


    
    if (method == "OPCV"){segmented = segmented_OPCV ;} 
    if (method == "perso"){segmented = segmented ;} // inutile en soit c'est juste pour qu'on oublie pas de rentrer un argument

    // Inverser les couleurs de l'image segmentée si nécessaire
    if (mean(segmented)[0] < 127) {
        for (int i = 0; i < segmented.rows; i++) {
            for (int j = 0; j < segmented.cols; j++) {
                segmented.at<uchar>(i, j) = 255 - segmented.at<uchar>(i, j);
            }
        }
    }

    // Calcul de données de comparaison si une image de référence est renseignée 
    if (!groundTruthFilename.empty()) {
        imageref.convertTo(imageref, CV_8U);
        
        double TP = 0, FP = 0, TN = 0, FN = 0;
        calculdonnees(segmented, imageref, TP, FP, TN, FN);

        double P = TP / (TP + FP);                // Précision
        double S = TP / (TP + FN);                // Sensibilité
        double DSC = 2 * TP / (2 * TP + FP + FN); // Coefficient de Similarité

        printf("Kmeans perso converge en %d itérations \n", N+1);
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

    image.convertTo(image, CV_8U);
    imshow("Image originale", image);
    if(!groundTruthFilename.empty()){imshow("Image de référence", imageref);}

    // Sauvegarde et affiche l'image
    imwrite("segmented_kmeans.png", segmented);
    imshow("Segmented image by kmeans", segmented);
    

    waitKey(0);

    destroyAllWindows();

    return EXIT_SUCCESS;
}
