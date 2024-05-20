#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int **matrix;
    int num_terms;
    int num_documents;
} TermDocumentMatrix;

TermDocumentMatrix* createTermDocumentMatrix(int num_terms, int num_documents) {
    TermDocumentMatrix* tdm = (TermDocumentMatrix*)malloc(sizeof(TermDocumentMatrix));
    tdm->num_terms = num_terms;
    tdm->num_documents = num_documents;
    tdm->matrix = (int**)malloc(num_terms * sizeof(int*));
    for (int i = 0; i < num_terms; i++) {
        tdm->matrix[i] = (int*)calloc(num_documents, sizeof(int));
    }
    return tdm;
}

void updateTermFrequency(TermDocumentMatrix* tdm, int term_index, int document_index, int frequency) {
    if (term_index < tdm->num_terms && document_index < tdm->num_documents) {
        tdm->matrix[term_index][document_index] = frequency;
    }
}

int getTermFrequency(TermDocumentMatrix* tdm, int term_index, int document_index) {
    if (term_index < tdm->num_terms && document_index < tdm->num_documents) {
        return tdm->matrix[term_index][document_index];
    }
    return -1;
}

int getDocumentFrequency(TermDocumentMatrix* tdm, int term_index) {
    if (term_index < tdm->num_terms) {
        int count = 0;
        for (int i = 0; i < tdm->num_documents; i++) {
            if (tdm->matrix[term_index][i] > 0) {
                count++;
            }
        }
        return count;
    }
    return -1;
}

int getTotalTermsInDocument(TermDocumentMatrix* tdm, int document_index) {
    if (document_index < tdm->num_documents) {
        int total = 0;
        for (int i = 0; i < tdm->num_terms; i++) {
            total += tdm->matrix[i][document_index];
        }
        return total;
    }
    return -1;
}

typedef struct {
    int row;
    int col;
    int value;
} SparseElement;

typedef struct {
    SparseElement* elements;
    int num_elements;
    int num_rows;
    int num_cols;
} SparseMatrix;

SparseMatrix* toSparseMatrix(TermDocumentMatrix* tdm) {
    SparseMatrix* sm = (SparseMatrix*)malloc(sizeof(SparseMatrix));
    sm->num_elements = 0;
    sm->num_rows = tdm->num_terms;
    sm->num_cols = tdm->num_documents;
    for (int i = 0; i < tdm->num_terms; i++) {
        for (int j = 0; j < tdm->num_documents; j++) {
            if (tdm->matrix[i][j] != 0) {
                sm->num_elements++;
            }
        }
    }
    sm->elements = (SparseElement*)malloc(sm->num_elements * sizeof(SparseElement));
    int k = 0;
    for (int i = 0; i < tdm->num_terms; i++) {
        for (int j = 0; j < tdm->num_documents; j++) {
            if (tdm->matrix[i][j] != 0) {
                sm->elements[k].row = i;
                sm->elements[k].col = j;
                sm->elements[k].value = tdm->matrix[i][j];
                k++;
            }
        }
    }
    return sm;
}

void freeTermDocumentMatrix(TermDocumentMatrix* tdm) {
    for (int i = 0; i < tdm->num_terms; i++) {
        free(tdm->matrix[i]);
    }
    free(tdm->matrix);
    free(tdm);
}

void freeSparseMatrix(SparseMatrix* sm) {
    free(sm->elements);
    free(sm);
}

int main() {
    TermDocumentMatrix* tdm = createTermDocumentMatrix(3, 3);
    updateTermFrequency(tdm, 0, 0, 1);
    updateTermFrequency(tdm, 1, 1, 2);
    updateTermFrequency(tdm, 2, 2, 3);
    
    SparseMatrix* sm = toSparseMatrix(tdm);
    
    for (int i = 0; i < sm->num_elements; i++) {
        printf("Element %d: row = %d, col = %d, value = %d\n", i, sm->elements[i].row, sm->elements[i].col, sm->elements[i].value);
    }
    
    freeTermDocumentMatrix(tdm);
    freeSparseMatrix(sm);
    
    return 0;
}
