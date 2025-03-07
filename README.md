<a id="readme-top"></a>

<!-- PROJECT BANNER --> 
<br /> 
<div align="center"> 
  <a href="https://github.com/votre-utilisateur/segmentation-texture"> 
    <img src="social_preview_segmentation.png"> </a> <h1 align="center">Segmentation d'Images de Texture</h1> 
  <p> <strong>Segmentation d'Images de Texture</strong> est un projet visant à segmenter des images de texture en utilisant les algorithmes K-means et Mean-Shift. Ce projet explore les techniques de segmentation fond/forme et en $N$ classes, tout en évaluant la qualité des résultats par rapport à des segmentations de référence. 
    
<br /> <a href="https://github.com/votre-utilisateur/segmentation-texture"><strong>Explorer »</strong></a> <br /> </p> </div> 
    
<!-- TABLE OF CONTENTS --> 
<details> <summary>Table des matières</summary> 
  
  <ol> <li> <a href="#about-the-project">À propos du projet</a> <ul> <li><a href="#repository-structure">Structure du repository</a></li> 
  <li><a href="#built-with">Outils utilisés</a></li> </ul> </li> <li> <a href="#getting-started">Pour commencer</a> <ul> <li><a href="#prerequisites">Prérequis</a></li>  
  <li><a href="#installation">Installation</a></li> </ul> </li> <li><a href="#usage">Utilisation</a></li> 
  <li><a href="#contact">Contacts</a></li> <li><a href="#acknowledgments">Sources</a></li> </ol> 
  
  </details> <h1 id="about-the-project">À propos du projet</h1> 
  
  <p> Ce projet se concentre sur la segmentation d'images de texture en utilisant les algorithmes K-means et Mean-Shift. L'objectif est de diviser les images en segments distincts, soit en fond et forme, soit en plusieurs classes, et d'évaluer la précision de ces segmentations par rapport à des références. </p> 
  
  <p> Fonctionnalités principales : 
    <br />- Segmentation fond/forme et en $N$ classes. 
    <br />- Évaluation des résultats avec des métriques de performance. 
    <br />- Comparaison des algorithmes K-means et Mean-Shift. 
    <br />- Visualisation des images segmentées. </p> 
    
<h2 id="repository-structure">📁 Structure du Repository</h2> 
<ul> 
  <li><code>data/images/</code> - Répertoire contenant les images de texture et les segmentations de référence.</li> 
  <li><code>src/</code> - Code source pour les implémentations de K-means et Mean-Shift.</li> </ul> 
  
<h2 id="built-with">🛠️ Conçu avec</h2> 
<ul> 
  <li>OpenCV</li> 
  <li>C</li> </ul> 
  
<h1 id="getting-started">🚀 Pour commencer</h1> <p>Suivez ces étapes pour configurer le projet en local :</p> 

<h3 id="prerequisites">⚙️ Prérequis</h3> 
<ul> 
  <li>OpenCV installé</li> 
  <li>Compilateur C++</li> </ul> 
  
<h3 id="installation">💻 Installation</h3> 
<ol> 
  <li>Clonez le dépôt : <pre><code>git clone https://github.com/votre-utilisateur/segmentation-texture.git</code></pre> </li> 
  <li>Accédez au répertoire du projet : <pre><code>cd segmentation-texture</code></pre> </li> 
  <li>Compilez le code : <pre><code>mkdir build && cd build && cmake .. && make</code></pre> </li> </ol> 
  
<h1 id="usage">⚡ Utilisation</h1> <p>Exécutez les commandes suivantes pour segmenter les images :</p> <pre><code>./kmeans ../data/images/texture1.png 2 ../data/images/texture1_ref.png</code></pre> <pre><code>./meanshift ../data/images/texture1.png 5 5 1.0 100</code></pre> 

<h1 id="contact">📞 Contact</h1> 
<ul> 
  <li><strong>Nom 1</strong> - <a href="https://linkedin.com/in/nom1" target="_blank">LinkedIn</a> - <a href="mailto\:nom1@example.com">nom1@example.com</a></li> 
  <li><strong>Nom 2</strong> - <a href="https://linkedin.com/in/nom2" target="_blank">LinkedIn</a> - <a href="mailto\:nom2@example.com">nom2@example.com</a></li> </ul> 
  
<p>Projet : <a href="https://github.com/votre-utilisateur/segmentation-texture" target="_blank">https://github.com/votre-utilisateur/segmentation-texture</a></p> 

<h1 id="acknowledgments">📚 Sources</h1> 
<ul> 
  <li><a href="https://docs.opencv.org/" target="_blank">Documentation OpenCV</a></li> <li><a href="https://isocpp.org/" target="_blank">Documentation C++</a></li> </ul> <p align="right">(<a href="#readme-top">retour en haut</a>)</p>
