#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "matrix_calc";

void print_matrix(const char *name, float mat[2][2])
{
    printf("%s:\n", name);
    printf("  [%7.2f, %7.2f]\n", mat[0][0], mat[0][1]);
    printf("  [%7.2f, %7.2f]\n\n", mat[1][0], mat[1][1]);
}

void read_serial_line(char *buffer, size_t max_len)
{
    while (true) {
        if (fgets(buffer, max_len, stdin) != NULL) {

            buffer[strcspn(buffer, "\r")] = 0;
            buffer[strcspn(buffer, "\n")] = 0;

            if (strlen(buffer) > 0) {
                return;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void app_main(void)
{
    char rx_buffer[128];
    float A[2][2], B[2][2], Res[2][2];

    ESP_LOGI(TAG, "Operaciones Matriciales");

    while (true) {
        printf("A? a11 a12 a21 a22: \n");
        read_serial_line(rx_buffer, sizeof(rx_buffer));
        if (sscanf(rx_buffer, "%f %f %f %f", &A[0][0], &A[0][1], &A[1][0], &A[1][1]) != 4) {
            printf("Error.\n\n");
            continue;
        }

        printf("B? b11 b12 b21 b22: \n");
        read_serial_line(rx_buffer, sizeof(rx_buffer));
        if (sscanf(rx_buffer, "%f %f %f %f", &B[0][0], &B[0][1], &B[1][0], &B[1][1]) != 4) {
            printf("Error.\n\n");
            continue;
        }

        printf("\n--- MATRICES ---\n");
        print_matrix("Matrix A", A);
        print_matrix("Matrix B", B);

        printf("--- RESULTS ---\n");

        // Suma (A + B)
        Res[0][0] = A[0][0] + B[0][0];
        Res[0][1] = A[0][1] + B[0][1];
        Res[1][0] = A[1][0] + B[1][0];
        Res[1][1] = A[1][1] + B[1][1];
        print_matrix("Addition (A + B)", Res);

        // Resta (A - B)
        Res[0][0] = A[0][0] - B[0][0];
        Res[0][1] = A[0][1] - B[0][1];
        Res[1][0] = A[1][0] - B[1][0];
        Res[1][1] = A[1][1] - B[1][1];
        print_matrix("Subtraction (A - B)", Res);
        
        printf("====================================\n\n");
    }
}