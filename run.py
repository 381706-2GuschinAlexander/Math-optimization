import subprocess as sp

def main():
    with(open("./build/result.txt", "w")) as f:
        for i,j in zip(range(0, 5000, 3), range(5000, 1, -3)):
            print(i, j)
            sp.run(["./build/main/console.exe", str(i), str(j)], stdout=f)

if  __name__ == '__main__':
    main()