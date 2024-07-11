#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_CLASSES 10
#define IMAGE_SIZE 784 // 28x28 pixels
#define IMAGE_WIDTH 28
#define IMAGE_HEIGHT 28

void load_mnist(const char *image_filename, const char *label_filename,
                double **images, int *labels, int num_images);
void train_model(double **images, int *labels, int num_images, double *weights);
int predict(double *image, double *weights);
double sigmoid(double x);
void normalize_image(double *image);
void display_image(double *image);
void test_and_display(double **images, int *labels, double *weights,
                      int num_images, int num_display);

void display_image(double *image) {
  for (int i = 0; i < IMAGE_HEIGHT; i++) {
    for (int j = 0; j < IMAGE_WIDTH; j++) {
      double pixel = image[i * IMAGE_WIDTH + j];
      if (pixel < 0.2)
        printf(" ");
      else if (pixel < 0.4)
        printf(".");
      else if (pixel < 0.6)
        printf("*");
      else if (pixel < 0.8)
        printf("O");
      else
        printf("#");
    }
    printf("\n");
  }
}

void test_and_display(double **images, int *labels, double *weights,
                      int num_images, int num_display) {
  int correct = 0;

  for (int i = 0; i < num_images; i++) {
    int prediction = predict(images[i], weights);
    if (prediction == labels[i]) {
      correct++;
    }

    if (i < num_display) {
      printf("\nTest Image %d:\n", i + 1);
      display_image(images[i]);
      printf("Predicted: %d, Actual: %d\n", prediction, labels[i]);
      printf(prediction == labels[i] ? "Correct!\n" : "Incorrect.\n");
    }
  }

  printf("\nOverall Accuracy: %.2f%%\n", (double)correct / num_images * 100);
}

void load_mnist(const char *image_filename, const char *label_filename,
                double **images, int *labels, int num_images) {
  FILE *image_file = fopen(image_filename, "rb");
  FILE *label_file = fopen(label_filename, "rb");

  if (image_file == NULL || label_file == NULL) {
    fprintf(stderr, "Error opening MNIST files\n");
    exit(1);
  }

  fseek(image_file, 16, SEEK_SET);
  fseek(label_file, 8, SEEK_SET);

  for (int i = 0; i < num_images; i++) {
    unsigned char pixel_buffer[IMAGE_SIZE];
    size_t read_size =
        fread(pixel_buffer, sizeof(unsigned char), IMAGE_SIZE, image_file);
    if (read_size != IMAGE_SIZE) {
      fprintf(stderr,
              "Error reading image file: expected %d bytes, got %zu bytes\n",
              IMAGE_SIZE, read_size);
      exit(1);
    }
    for (int j = 0; j < IMAGE_SIZE; j++) {
      images[i][j] = (double)pixel_buffer[j] / 255.0; // Normalize to [0, 1]
    }

    unsigned char label;
    if (fread(&label, sizeof(unsigned char), 1, label_file) != 1) {
      fprintf(stderr, "Error reading label file\n");
      exit(1);
    }
    labels[i] = (int)label;
  }

  fclose(image_file);
  fclose(label_file);
}

void train_model(double **images, int *labels, int num_images,
                 double *weights) {
  double learning_rate = 0.01;
  int epochs = 5;

  for (int epoch = 0; epoch < epochs; epoch++) {
    for (int i = 0; i < num_images; i++) {
      double *image = images[i];
      int true_label = labels[i];

      // Forward pass
      double logits[NUM_CLASSES] = {0};
      for (int j = 0; j < NUM_CLASSES; j++) {
        for (int k = 0; k < IMAGE_SIZE; k++) {
          logits[j] += weights[j * IMAGE_SIZE + k] * image[k];
        }
      }

      double max_logit = logits[0];
      for (int j = 1; j < NUM_CLASSES; j++) {
        if (logits[j] > max_logit)
          max_logit = logits[j];
      }
      double sum_exp = 0;
      for (int j = 0; j < NUM_CLASSES; j++) {
        logits[j] = exp(logits[j] - max_logit);
        sum_exp += logits[j];
      }
      for (int j = 0; j < NUM_CLASSES; j++) {
        logits[j] /= sum_exp;
      }

      for (int j = 0; j < NUM_CLASSES; j++) {
        double error = (j == true_label ? 1.0 : 0.0) - logits[j];
        for (int k = 0; k < IMAGE_SIZE; k++) {
          weights[j * IMAGE_SIZE + k] += learning_rate * error * image[k];
        }
      }
    }

    if ((epoch + 1) % 1 == 0) {
      printf("Epoch %d completed\n", epoch + 1);
    }
  }
}

int predict(double *image, double *weights) {
  double logits[NUM_CLASSES] = {0};
  for (int j = 0; j < NUM_CLASSES; j++) {
    for (int k = 0; k < IMAGE_SIZE; k++) {
      logits[j] += weights[j * IMAGE_SIZE + k] * image[k];
    }
  }

  int max_class = 0;
  for (int j = 1; j < NUM_CLASSES; j++) {
    if (logits[j] > logits[max_class]) {
      max_class = j;
    }
  }

  return max_class;
}

double sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }

void normalize_image(double *image) {
  double mean = 0.0;
  double std_dev = 0.0;

  for (int i = 0; i < IMAGE_SIZE; i++) {
    mean += image[i];
  }
  mean /= IMAGE_SIZE;

  for (int i = 0; i < IMAGE_SIZE; i++) {
    std_dev += pow(image[i] - mean, 2);
  }
  std_dev = sqrt(std_dev / IMAGE_SIZE);

  for (int i = 0; i < IMAGE_SIZE; i++) {
    image[i] = (image[i] - mean) / (std_dev + 1e-8);
  }
}

int main() {
  int num_train = 60000;
  int num_test = 10000;
  double **train_images = malloc(num_train * sizeof(double *));
  int *train_labels = malloc(num_train * sizeof(int));
  double **test_images = malloc(num_test * sizeof(double *));
  int *test_labels = malloc(num_test * sizeof(int));
  double *weights = calloc(NUM_CLASSES * IMAGE_SIZE, sizeof(double));

  for (int i = 0; i < num_train; i++) {
    train_images[i] = malloc(IMAGE_SIZE * sizeof(double));
  }
  for (int i = 0; i < num_test; i++) {
    test_images[i] = malloc(IMAGE_SIZE * sizeof(double));
  }

  load_mnist("../data/train-images.idx3-ubyte",
             "../data/train-labels.idx1-ubyte", train_images, train_labels,
             num_train);
  load_mnist("../data/t10k-images.idx3-ubyte", "../data/t10k-labels.idx1-ubyte",
             test_images, test_labels, num_test);

  train_model(train_images, train_labels, num_train, weights);

  int num_display = 10;
  test_and_display(test_images, test_labels, weights, num_test, num_display);

  for (int i = 0; i < num_train; i++)
    free(train_images[i]);
  for (int i = 0; i < num_test; i++)
    free(test_images[i]);
  free(train_images);
  free(test_images);
  free(train_labels);
  free(test_labels);
  free(weights);

  return 0;
}
