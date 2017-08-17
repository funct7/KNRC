/* qsort:  sort v[left]...v[right] into increasing order */
void qsort(char *v[], int left, int right) {
    int i, last;
    void swap(char *v[], int i, int j);
    int _strcmp(char *s, char *t);

    if (left >= right) {
        return;
    }

    swap(v, left, (left+right)/2);
    last = left;
    for (i = left+1; i <= right; i++) {
        if (_strcmp(v[i], v[left]) < 0) {
            swap(v, ++last, i);
        }
    }
    swap(v, left, last);
    qsort(v, left, last-1);
    qsort(v, last+1, right);
}
