# 💡 GPGPU Rendering Techniques - Illumination – Materials

Projet académique réalisé à **CPE Lyon (5ETI – IMI 2024-2025)**.  
L’objectif est d’exploiter la puissance de la programmation **GPU** pour mettre en œuvre des techniques de rendu temps réel, en utilisant **OpenGL** et des **shaders GLSL**.

---

## ✨ Fonctionnalités implémentées

- **Illumination de Phong** : modèle d’éclairage classique avec composantes ambiante, diffuse et spéculaire.  
  ![Phong](images-readme/phong.png)

- **Multi-textures & Occlusion ambiante (AO)** : enrichissement visuel grâce à la combinaison de cartes de matériaux.  
  ![AO](images-readme/ao.png)

- **Displacement mapping** : modification dynamique de la géométrie à partir d’une carte de hauteur.  
  ![Displacement](images-readme/displacement.png)

- **Normal mapping (espace tangent)** : simulation de détails de surface sans augmenter le nombre de sommets.  
  ![Normal Mapping](images-readme/normal_mapping.png)

---

## 🛠️ Stack technique

- **Langage** : C / C++  
- **API graphique** : OpenGL  
- **Shaders** : GLSL  
- **Pipeline** : Vertex Shader + Fragment Shader  
- **Outils** : GLFW, GLAD, stb_image  

---

## 🚀 Lancer le projet

```bash
# Cloner le repo
git clone https://github.com/MathieuZem/GPGPU-Rendering-Techniques.git
cd GPGPU-Rendering-Techniques

# Compiler et exécuter (exemple avec CMake)
mkdir build && cd build
cmake ..
make
./gpu_illumination
