# Crypter - Payload File Version




The build process now requires an additional step to prepare the external payload.

### Step 1: Encrypt the Payload

```bash
python UUIDEncrypter.py havoc.bin payload.enc
```

### Step 2: Build the Artifact
```bash
cl Crypter.c
.\Crypter.exe
```

### Step 3: Execution

```bash
.\CrypterArtifact.exe payload.enc
```


GCC Compile Command (For the Builder)
To compile the modularized builder program on your host using GCC:

bash
gcc -I./include src/main.c src/builder.c -o CrypterBuilder
