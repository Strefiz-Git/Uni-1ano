int f(int x) { 
    x += 5; 
    return x; 
} 

int main(void) { 
    int a = 2; 
    int b; 
    b = f(a);
    printf("%d %d", a, b); 
    return 0; 
}