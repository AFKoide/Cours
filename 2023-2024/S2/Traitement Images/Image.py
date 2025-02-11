# Déclaration des bibliothèques
import cv2
from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
import math

Cobaye = Image.open(r'./images/chat.jpg')

def Teinte(image):
    luminance = image.convert("L") if image.mode != "L" else image

    luminance.save(r"chat gris.jpg")


    HSV = image.convert("HSV")
    canaux = HSV.split()
    canaux[0].save(r"chat HSV.jpg")


# Teinte(Cobaye)



"""Histogramme"""
def Histo(img_array):
    niveaux = np.zeros(256)
    for pixel_value in img_array:
        niveaux[pixel_value]+= 1
    print(niveaux)


    return niveaux



def Histogramme(image, process, display):
    match process:
        case "RGB":
            if image.mode != "RGB":
                print(f"L'image n'est pas en RGB : elle est en {image.mode}")
                return None

            canaux = Image.Image.split(image)
            pixels_r = list(canaux[0].getdata()); data_r, bins_r, _ = plt.hist(pixels_r, bins=255, alpha=0.5, color='red',   label='Red Channel')
            pixels_g =list(canaux[1].getdata());  data_g, bins_g, _ = plt.hist(pixels_g, bins=255, alpha=0.5, color='green', label='Green Channel')
            pixels_b =list(canaux[2].getdata());  data_b, bins_b, _ = plt.hist(pixels_b, bins=255, alpha=0.5, color='blue',  label='Blue Channel')
            plt.xlabel("Niveau de couleur"); plt.ylabel("Nbr de pixels")

            if display is True:
                plt.show()
    
            return data_r, data_g, data_b, bins_r

        case "L":
            luminance = image.convert("L") if image.mode != "L" else image

            pixels_l = list(luminance.getdata()); data_l, bins_l, _ = plt.hist(pixels_l, bins=255, color='k', label='Luminance Channel')
            plt.xlabel("Niveau de gris"); plt.ylabel("Nbr de pixels")

            if display is True:
                plt.show()
    
            return data_l, bins_l


def Histogramme_Cumulee(image, process, display):
    match process:
        case "RGB":
            if image.mode != "RGB":
                print(f"L'image n'est pas en RGB : elle est en {image.mode}")
                return None

            canaux = Image.Image.split(image)
            plt.subplot(4,1,1)
            pixels_r = list(canaux[0].getdata()); data_r, bins_r, _ = plt.hist(pixels_r, bins=255, cumulative=True, alpha=0.5, color='red', label='Red Channel')
            plt.xlabel("Niveau de couleur"); plt.ylabel("Nbr de pixels rouge")
            plt.subplot(4,1,2)
            pixels_g =list(canaux[1].getdata());  data_g, bins_g, _ = plt.hist(pixels_g, bins=255, cumulative=True, alpha=0.5, color='green', label='Green Channel')
            plt.xlabel("Niveau de couleur"); plt.ylabel("Nbr de pixels vert")
            plt.subplot(4,1,3)
            pixels_b =list(canaux[2].getdata());  data_b, bins_b, _ = plt.hist(pixels_b, bins=255, cumulative=True, alpha=0.5, color='blue', label='Blue Channel')
            plt.xlabel("Niveau de couleur"); plt.ylabel("Nbr de pixels bleu")
            
            if display is True:
                plt.show()

            return data_r, bins_r, data_g, data_b

        case "L":
            luminance = image.convert("L") if image.mode != "L" else image

            pixels_l = list(luminance.getdata()); data_l, bins_l, _ = plt.hist(pixels_l, bins=255, cumulative=True, color='k', label='Luminance Channel')
            plt.xlabel("Niveau de gris"); plt.ylabel("Nbr de pixels cumulés")

            if display is True:
                plt.show()

            return data_l, bins_l


def Rapport_Histo(image):
    plt.subplot(2,1,1)
    Histogramme(image, "RGB", False)

    plt.subplot(2,1,2)
    Histogramme(image, "L", False)
    plt.show()

    plt.subplot(4,1,1)
    Histogramme_Cumulee(image, "RGB", False)
    
    plt.subplot(4,1,4)
    Histogramme_Cumulee(image, "L", False)
    plt.show()

# Rapport_Histo(Cobaye)





"""EGALISEUR"""
def Egaliseur(image, process):
    match process:
        case "L":
            luminance = image.convert("L") if image.mode != "L" else image
            pixels_l = list(luminance.getdata())
            N = len(pixels_l)

            # Histogramme Cumulatif
            hist_cumul_l, bins = Histogramme_Cumulee(image, "L", True)

            # Egalisation:
            egalise = np.zeros(N)
            for i in range(N):
                egalise[i] = round((255/N) * hist_cumul_l[pixels_l[i]])

            image_egalise = Image.new("L", image.size)
            image_egalise.putdata(egalise)
            image_egalise.save(r'./images/chat_egalise.jpg')

            Histogramme_Cumulee(Image.open(r'./images/chat_egalise.jpg'), "L", True)


        case "RGB":
            if image.mode != "RGB":
                print(f"L'image n'est pas en RGB : elle est en {image.mode}")
                return None
            
            pixels_r = list(image.getchannel('R').getdata())
            pixels_g = list(image.getchannel('G').getdata())
            pixels_b = list(image.getchannel('B').getdata())
            N = len(pixels_r)
            print(N)
            # Histogramme Cumulatif
            hist_cumul_r, hist_cumul_g, hist_cumul_b, bins = Histogramme_Cumulee(image, "RGB", True)

            # Egalisation:
            egalise_r = np.zeros(N) ; egalise_g = np.zeros(N) ; egalise_b = np.zeros(N)
            for i in range(N):
                egalise_r[i] = round((255/N) * hist_cumul_r[pixels_r[i]])
                egalise_g[i] = round((255/N) * hist_cumul_g[pixels_g[i]])
                egalise_b[i] = round((255/N) * hist_cumul_b[pixels_b[i]])

            # Merge the equalized channels back into an image
            image_egalise = Image.new("RGB", image.size)
            image_egalise = Image.merge("RGB", (
                Image.fromarray(egalise_r.reshape(image.size[::-1])).convert('L'),
                Image.fromarray(egalise_g.reshape(image.size[::-1])).convert('L'),
                Image.fromarray(egalise_b.reshape(image.size[::-1])).convert('L')
            ))
            
            # Enregistre l'image
            image_egalise.save(r'equalized_image.jpg')
            image_egalise.show()


# Egaliseur(Image.open(r'./images/chat.jpg'), "RGB")





"""BINARISATION"""
def Binarisation(image, niveau):
    print("Lancement Binarisation")
    luminance = image.convert("L")

    binarisation = np.where(np.array(luminance) < niveau, 0, 255)

    image_binarise = Image.fromarray(binarisation.astype(np.uint8))
    image_binarise.save('chat binarise.jpg')
    print(f"Fin de la binarisation selon le paramètre {niveau}")
    image_binarise.show()
    return np.array(image_binarise)


def Binarisation_Otsu(source):
    luminance = source.convert("L")
    pixels_l = list(luminance.getdata())

    histogramme, bins, _ = plt.hist(pixels_l, bins=256)
    cumsum = np.cumsum(histogramme)                 # Histogramme cumulée
    N = cumsum[-1]
    index_10 = np.argmax(cumsum >= 0.10* N)
    index_90 = np.argmax(cumsum >= 0.90 * N)


    D = np.zeros(255)
    print(f"Indice 1: {index_10} et indice 90: {index_90}")

    for i in range(index_10, index_90):
        echantillon_1 = histogramme[0:i] ; echantillon_2 = histogramme[i:255]
        n1 = np.sum(echantillon_1)
        n2 = np.sum(echantillon_2)


        m1 = 0
        for x in range(0, len(echantillon_1)):
            m1 += x * histogramme[x]
        m1 = m1/n1 if n1 != 0 else 0

        m2 = 0
        for x in range(len(echantillon_1), 255):
            m2 += x * histogramme[x]
        m2 = m2/n2 if n2 != 0 else 0


        var1 = 0
        for x in range(0, len(echantillon_1)):
            var1 = var1 + (x-m1)**2 * histogramme[x]
        var1 = var1/n1 if n1 != 0 else 1

        var2 = 0
        for x in range(len(echantillon_2), 255):
            var2 = var2 + (x-m2)**2 * histogramme[x]
        var2 = var2/n2 if n2 != 0 else 1


        print(f"m1: {m1}, m2: {m2}, var1: {var1}, var2: {var2}")
        D[i] = ((n1 + n2)*(m1 - m2)**2)/((n1*var1)+(n2*var2))


    seuil_otsu = np.argmax(D) + index_10
    print(seuil_otsu)
    plt.vlines(seuil_otsu, 0, max(histogramme), colors="k") ; plt.vlines(index_10, 0, max(histogramme), colors="r") ; plt.vlines(index_90, 0, max(histogramme), colors="r")
    plt.xlabel("Niveau de Gris") ; plt.ylabel("Nombre de Pixel")

    plt.twinx()
    plt.plot(np.arange(len(D)) + index_10, D, "k", label="D")
    plt.show()

    return Binarisation(source, seuil_otsu)


def Otsu_Opencv(image):
    # Charger l'image en niveaux de gris
    image = cv2.imread(r'./images/chat.jpg', cv2.IMREAD_GRAYSCALE)

    # Appliquer la méthode d'Otsu pour trouver le seuil optimal
    seuil, image_binaire = cv2.threshold(image, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)

    print("Seuil d'Otsu trouvé :", seuil)

    # Afficher l'image binaire résultante
    cv2.imshow('Image binaire', image_binaire)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    # Calculer la variance en fonction des points sur l'image
    variance = np.zeros(256)
    for seuil in range(256):
        mask = image <= seuil
        background = image[mask]
        objet = image[~mask]
        variance[seuil] = np.var(background) * background.size + np.var(objet) * objet.size

    # Afficher l'histogramme de l'image
    histo = plt.hist(image.ravel(), bins=256, range=[0,256], label="Histogramme")
    plt.title("Histogramme de l'image & Variance en fonction du seuil")
    plt.vlines(80, ymin=0, ymax=10000, colors="k", label="Seuil")
    plt.legend()

    plt.twinx()
    # Afficher la variance en fonction des seuils
    plt.plot(range(0, 256), variance, 'r', label="Variance")
    plt.title("")
    plt.xlabel("Seuil")
    plt.ylabel("Variance")
    plt.legend()
    plt.show()


# Binarisation(Cobaye, 100)

# Binarisation_Otsu(Image.open(r'./images/chat.jpg'))





"""FERMETURE"""
def Dilatation(pixel): # Recoit les valeurs des pixels
    print("Dilatation")
    # Kernel de Dilatation
    kernel = np.array([[0, 1, 0],
                       [1, 1, 1],
                       [0, 1, 0]])

    # Pad the image to handle borders
    pad_width = kernel.shape[0] // 2
    padded_img = np.pad(pixel, pad_width, mode='constant')

    # Create an empty array similar to the previous image
    result = np.zeros_like(pixel)

    # Iterate over each pixel in the image
    for i in range(pixel.shape[0]):
        for j in range(pixel.shape[1]):
            # Apply the kernel to the neighborhood around the pixel
            neighborhood = padded_img[i:i+kernel.shape[0], j:j+kernel.shape[1]]
            result[i, j] = np.max(neighborhood * kernel)

    # Convert result back to PIL image
    # dilated_image = Image.fromarray(result)
    # dilated_image.show()

    return result

def Erosion(pixel): # Recoit les valeurs des pixels
    print("Erosion")
    # Kernel d'Erosion
    kernel = np.array([[1, 1, 1],
                       [1, 1, 1],
                       [1, 1, 1]])

    # Pad the image to handle borders
    pad_width = kernel.shape[0] // 2
    padded_img = np.pad(pixel, pad_width, mode='constant')

    # Create an empty array similar to the previous image
    result = np.zeros_like(pixel)

    # Iterate over each pixel in the image
    for i in range(pixel.shape[0]):
        for j in range(pixel.shape[1]):
            # Apply the kernel to the neighborhood around the pixel
            neighborhood = padded_img[i:i+kernel.shape[0], j:j+kernel.shape[1]]
            result[i, j] = np.max(neighborhood * kernel)

    # Convert result back to PIL image
    # eroded_image = Image.fromarray(result)
    # eroded_image.show()

    return result


def Fermeture(image, nb):
    img_array = Binarisation(image, 80)

    for i in range(nb):
        img_array = Dilatation(img_array)
        img_array = Erosion(img_array)

    resulted_image = Image.fromarray(img_array)
    resulted_image.show()
    resulted_image.save(r"Fermeture.jpg")


def Ouverture(image, nb):
    img_array = Binarisation(image, 121)

    for i in range(nb):
        img_array = Erosion(img_array)
        img_array = Dilatation(img_array)

    resulted_image = Image.fromarray(img_array)
    resulted_image.show()
    resulted_image.save(r"Ouverture.jpg")


# Fermeture(Cobaye, 2)
# Ouverture(Cobaye, 2)




"""HARRIS"""
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

    seuil = 75
    Qt = np.where(Q < seuil, 255, 0)
    print(Qt)

    harriser = Image.fromarray(Qt.astype(np.uint8))
    harriser.show()
    
    
    # harriser.save("Image Harris.jpg")
    # Image.fromarray(Ix).save("Ix.jpg")
    # Image.fromarray(Iy).save("Iy.jpg")
    # Image.fromarray(image_floutee).save("Flou.jpg")

Cobaye = Image.open(r'./images/tool.jpg')

Harris(Cobaye)


# http://operationpixel.free.fr/traitementniveaudegris_corner.php