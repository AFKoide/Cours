# Déclaration des bibliothèques
from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
import math

def Convolution(image, kernel):
    # luminance = image.convert("L") if image.mode != "L" else image

    # Convertir l'image en tableau NumPy
    img_array = np.array(image)

    # Dimensions de l'image et du noyau
    height, width = img_array.shape[:2]
    k_height, k_width = kernel.shape

    # Bordure du noyau
    pad_height = k_height // 2
    pad_width = k_width // 2

    # Créer une nouvelle image pour stocker le résultat de la convolution
    result = np.zeros_like(img_array)

    # Padder l'image pour gérer les bords
    padded_img = np.pad(img_array, ((pad_height, pad_height), (pad_width, pad_width)), mode='constant')

    # Appliquer la convolution
    for i in range(height):
        for j in range(width):
            # Extraire la région de l'image correspondant au noyau
            region = padded_img[i:i + k_height, j:j + k_width]
            # Appliquer le noyau en effectuant une multiplication matricielle
            result[i, j] = np.sum(region * kernel)

    # Convertir le résultat en une image PIL
    # convolved_image = Image.fromarray(result.astype(np.uint8))
    # convolved_image.show()
    # return convolved_image

    return result




def Harris(image):
    # kernel_gauss = np.array([[1, 2, 1],
    #                          [2, 4, 2],
    #                          [1, 2, 1]]) / 16

    kernel_gauss = np.array([[1,  4,  7,  4,  1],
                            [4, 16, 26, 16,  4],
                            [7, 26, 41, 26,  7],
                            [4, 16, 26, 16,  4],
                            [1,  4,  7,  4,  1]]) / 273

    
    # kernel_gauss = np.array([[ 0,  0,  1,  2,  1,  0,  0],
    #                          [ 0,  3, 13, 22, 13,  3,  0],
    #                          [ 1, 13, 59, 97, 59, 13,  1],
    #                          [ 2, 22, 97,159, 97, 22,  2],
    #                          [ 1, 13, 59, 97, 59, 13,  1],
    #                          [ 0,  3, 13, 22, 13,  3,  0],
    #                          [ 0,  0,  1,  2,  1,  0,  0]]) / 1003

    kernel_x = np.array([[-1, 0, 1]]) / 30
    kernel_y = np.array([[-1],
                         [0],
                         [1]]) / 30

    image_floutee = Convolution(image.convert("L"), kernel_gauss);     print("Gauss de image fait")
    Ix = Convolution(image_floutee, kernel_x);                         print("Ix fait")
    Iy = Convolution(image_floutee, kernel_y);                         print("Iy fait")

    A = Ix * Ix; At = Convolution(A, kernel_gauss);                    print("Gauss de A fait")
    B = Iy * Iy; Bt = Convolution(B, kernel_gauss);                    print("Gauss de B fait")
    C = Ix * Iy; Ct = Convolution(C, kernel_gauss);                    print("Gauss de C fait")

    Q = (At * Bt - Ct)**2 - 0.06 * (At + Bt)**2 
    print(Q)

    seuil = 200
    Qt = np.where(Q < seuil, 0, 255)
    print(Qt)

    harriser = Image.fromarray(Qt.astype(np.uint8))
    harriser.show()

Cobaye = Image.open(r'./images/tool.jpg')

Harris(Cobaye)

# http://operationpixel.free.fr/traitementniveaudegris_corner.php