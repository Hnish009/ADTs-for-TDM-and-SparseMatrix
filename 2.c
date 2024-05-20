#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int row;
    int col;
    int value;
} SparseElement;

typedef struct {
    SparseElement* elements;
    int num_elements;
    int rows;
    int cols;
} SparseArray;

SparseArray* createSparseArray(int rows, int cols) {
    SparseArray* sa = (SparseArray*)malloc(sizeof(SparseArray));
    sa->elements = NULL;
    sa->num_elements = 0;
    sa->rows = rows;
    sa->cols = cols;
    return sa;
}

void setValue(SparseArray* sa, int row, int col, int value) {
    for (int i = 0; i < sa->num_elements; i++) {
        if (sa->elements[i].row == row && sa->elements[i].col == col) {
            sa->elements[i].value = value;
            return;
        }
    }
    sa->num_elements++;
    sa->elements = (SparseElement*)realloc(sa->elements, sa->num_elements * sizeof(SparseElement));
    sa->elements[sa->num_elements - 1].row = row;
    sa->elements[sa->num_elements - 1].col = col;
    sa->elements[sa->num_elements - 1].value = value;
}

int getValue(SparseArray* sa, int row, int col) {
    for (int i = 0; i < sa->num_elements; i++) {
        if (sa->elements[i].row == row && sa->elements[i].col == col) {
            return sa->elements[i].value;
        }
    }
    return 0;
}

void addValue(SparseArray* sa, int row, int col, int value) {
    for (int i = 0; i < sa->num_elements; i++) {
        if (sa->elements[i].row == row && sa->elements[i].col == col) {
            sa->elements[i].value += value;
            return;
        }
    }
    sa->num_elements++;
    sa->elements = (SparseElement*)realloc(sa->elements, sa->num_elements * sizeof(SparseElement));
    sa->elements[sa->num_elements - 1].row = row;
    sa->elements[sa->num_elements - 1].col = col;
    sa->elements[sa->num_elements - 1].value = value;
}

void removeValue(SparseArray* sa, int row, int col) {
    for (int i = 0; i < sa->num_elements; i++) {
        if (sa->elements[i].row == row && sa->elements[i].col == col) {
            for (int j = i; j < sa->num_elements - 1; j++) {
                sa->elements[j] = sa->elements[j + 1];
            }
            sa->num_elements--;
            sa->elements = (SparseElement*)realloc(sa->elements, sa->num_elements * sizeof(SparseElement));
            return;
        }
    }
}

int getNumRows(SparseArray* sa) {
    return sa->rows;
}

int getNumCols(SparseArray* sa) {
    return sa->cols;
}

SparseArray* transpose(SparseArray* sa) {
    SparseArray* transposed = createSparseArray(sa->cols, sa->rows);
    for (int i = 0; i < sa->num_elements; i++) {
        setValue(transposed, sa->elements[i].col, sa->elements[i].row, sa->elements[i].value);
    }
    return transposed;
}

void multiplyScalar(SparseArray* sa, int factor) {
    for (int i = 0; i < sa->num_elements; i++) {
        sa->elements[i].value *= factor;
    }
}

SparseArray* add(SparseArray* sa1, SparseArray* sa2) {
    if (sa1->rows != sa2->rows || sa1->cols != sa2->cols) return NULL;
    SparseArray* result = createSparseArray(sa1->rows, sa1->cols);
    for (int i = 0; i < sa1->num_elements; i++) {
        setValue(result, sa1->elements[i].row, sa1->elements[i].col, sa1->elements[i].value);
    }
    for (int i = 0; i < sa2->num_elements; i++) {
        addValue(result, sa2->elements[i].row, sa2->elements[i].col, sa2->elements[i].value);
    }
    return result;
}

SparseArray* multiply(SparseArray* sa1, SparseArray* sa2) {
    if (sa1->cols != sa2->rows) return NULL;
    SparseArray* result = createSparseArray(sa1->rows, sa2->cols);
    for (int i = 0; i < sa1->num_elements; i++) {
        for (int j = 0; j < sa2->num_elements; j++) {
            if (sa1->elements[i].col == sa2->elements[j].row) {
                addValue(result, sa1->elements[i].row, sa2->elements[j].col, sa1->elements[i].value * sa2->elements[j].value);
            }
        }
    }
    return result;
}

int* toDenseArray(SparseArray* sa) {
    int* dense = (int*)calloc(sa->rows * sa->cols, sizeof(int));
    for (int i = 0; i < sa->num_elements; i++) {
        dense[sa->elements[i].row * sa->cols + sa->elements[i].col] = sa->elements[i].value;
    }
    return dense;
}

void freeSparseArray(SparseArray* sa) {
    free(sa->elements);
    free(sa);
}

int main() {
    SparseArray* sa = createSparseArray(3, 3);
    setValue(sa, 0, 0, 1);
    setValue(sa, 1, 1, 2);
    setValue(sa, 2, 2, 3);

    int* dense = toDenseArray(sa);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", dense[i * 3 + j]);
        }
        printf("\n");
    }

    free(dense);
    freeSparseArray(sa);
    return 0;
}

