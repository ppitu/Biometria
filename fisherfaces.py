import cv2
import cv2.face
import numpy as np

def norm_0_255(src):
    # Create and return normalized image:
    dst = np.zeros_like(src)
    if src.ndim == 2:
        cv2.normalize(src, dst, 0, 255, cv2.NORM_MINMAX, cv2.CV_8UC1)
    elif src.ndim == 3:
        cv2.normalize(src, dst, 0, 255, cv2.NORM_MINMAX, cv2.CV_8UC3)
    else:
        src.copyTo(dst)
    return dst

def read_csv(filename, separator=';'):
    images = []
    labels = []
    with open(filename, 'r') as file:
        lines = file.readlines()
        for line in lines:
            path, classlabel = line.strip().split(separator)
            if path and classlabel:
                image = cv2.imread(path, 0)
                images.append(image)
                labels.append(int(classlabel))
    return images, labels

if __name__ == '__main__':
    import sys

    # Check for valid command line arguments, print usage
    # if no arguments were given.
    if len(sys.argv) < 2:
        print(f"usage: {sys.argv[0]} <csv.ext> <output_folder>")
        sys.exit(1)

    output_folder = "."
    if len(sys.argv) == 3:
        output_folder = sys.argv[2]

    # Get the path to the CSV file.
    fn_csv = sys.argv[1]

    # These lists hold the images and corresponding labels.
    images, labels = read_csv(fn_csv)

    # Quit if there are not enough images for this demo.
    if len(images) <= 1:
        error_message = "This demo needs at least 2 images to work. Please add more images to your data set!"
        raise Exception(error_message)

    # Get the height from the first image. We'll need this
    # later in code to reshape the images to their original size.
    height, _ = images[0].shape

    # The following lines create a Fisherfaces model for
    # face recognition and train it with the images and
    # labels read from the given CSV file.
    model = cv2.face.FisherFaceRecognizer_create()
    model.train(images, np.array(labels))

    # The following line predicts the label of a given
    # test image.
    testSample = images[-1]
    testLabel = labels[-1]
    predictedLabel, _ = model.predict(testSample)

    result_message = f"Predicted class = {predictedLabel} / Actual class = {testLabel}."
    print(result_message)

    # Here is how to get the eigenvalues of this Fisherfaces model:
    eigenvalues = model.getEigenValues()

    # And we can do the same to display the Eigenvectors (read Eigenfaces):
    W = model.getEigenVectors()

    # Get the sample mean from the training data
    mean = model.getMean()

    # Display or save the mean image
    if len(sys.argv) == 2:
        cv2.imshow("mean", norm_0_255(mean.reshape(1, height)))
    else:
        cv2.imwrite(f"{output_folder}/mean.png", norm_0_255(mean.reshape(1, height)))

    # Display or save the Fisherfaces
    for i in range(min(16, W.shape[1])):
        eigenvalue = eigenvalues[i]
        ev = W[:, i].clone()

        # Reshape to original size & normalize to [0...255] for imshow.
        grayscale = norm_0_255(ev.reshape(-1, height))
        # Show the image & apply a Bone colormap for better visualization.
        cgrayscale = cv2.applyColorMap(grayscale, cv2.COLORMAP_BONE)

        # Display or save the Fisherface image
        if len(sys.argv) == 2:
            cv2.imshow(f"fisherface_{i}", cgrayscale)
        else:
            cv2.imwrite(f"{output_folder}/fisherface_{i}.png", norm_0_255(cgrayscale))

    # Display or save the image reconstructions at some predefined steps
    for num_component in range(min(16, W.shape[1])):
        ev = W[:, num_component]
        projection = cv2.face.LBPHFaceRecognizer.subspaceProject(ev, mean, images[0].reshape(1, -1))
        reconstruction = cv2.face.LBPHFaceRecognizer.subspaceReconstruct(ev, mean, projection)

        # Normalize the result
        reconstruction = norm_0_255(reconstruction.reshape(-1, height))

        # Display or save the image reconstruction
        if len(sys.argv) == 2:
            cv2.imshow(f"fisherface_reconstruction_{num_component}", reconstruction)
        else:
            cv2.imwrite(f"{output_folder}/fisherface_reconstruction_{num_component}.png", reconstruction)

    # Display if we are not writing to an output folder
    if len(sys.argv) == 2:
        cv2.waitKey(0)