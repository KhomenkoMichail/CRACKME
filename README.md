# Educational task "CRACKME"  

## Description  
The program asks for a password. If the user enters the correct password, the phrase "access granted!" appears; otherwise the phrase "access denied." is displayed.  

The program contains two specially created vulnerabilities that cause the program to display the "access granted!" phrase when an incorrect password is entered.  

The educational task was to create such a program and to find vulnerabilities in your partner's program. 

## My program  

You can see the code of my program in the file "creackme.asm".

It displays a red frame with a message when an incorrect password is entered, and a green frame otherwise.

<img width="1023" height="649" alt="image" src="https://github.com/user-attachments/assets/215533fd-5f20-43f7-87be-ae2c5cd71cef" />

<img width="1023" height="640" alt="image" src="https://github.com/user-attachments/assets/bb01b7be-2af3-4e51-b3f1-d1c1814e75f2" />

### Program vulnerabilities:  
#### 1. Buffer overflow  
The password entered by the user is processed using 01 function of the 21 interrupt and is written character by character into the buffer located before the program code.  

Despite the 256-byte buffer size, the program doesn't check the length of the entered password. Therefore, the entered password could exceed the buffer's boundaries and alter the program code, for example, replacing the password check with "NOP."  

```
bufferData                  db 256 dup(0)

main:
                            cld

                            mov ah, 09h
                            mov dx, offset requestMessage
                            int 21h

                            call getPassword

                            push cx
                            call cmpPasswords

                            cmp dx, PASSWORD_SUCCESS
                            je grant
                            call    printfFailMessage

grant:                      call    printfSuccessMessage

endProgram:
                            mov ax, 4c00h
                            int 21h
```

#### 2. 8-bit register integer overflow

The function "cmpPasswords" uses stack-based argument passing in the Pascal convention. Еhe only argument it takes is the length of the string entered by the user. Then it allocates an array on the stack large enough to save the user's password.

The password length is placed in the al register, which is incremented and subtracted from the sp register. The new sp value is used as the address to copy the user password from the buffer.

When entering a 255-character password, the al register overflows and no memory is allocated on the stack, while 255 bytes are copied from the password buffer using the sp pointer, which can change the function return address.

```
cmpPasswords                proc
                            push bp
                            mov bp, sp

                            xor ax, ax
                            mov byte ptr al, [bp + 4]
                            add al, 1
                            sub sp, ax

                            lea si, bufferData
                            mov di, sp
                            mov cx, [bp+4]
                            rep movsb

                            mov cx, [bp+4]
                            mov si, sp
                            call getStringHash

                            mov bx, ax

                            mov cl, PASSWORD_LENGTH
                            lea si, password
                            push bx
                            call getStringHash
                            pop bx

                            xor dx, dx
                            cmp ax, bx
                            jne @@end

                            push cs
                            pop es

                            push cs
                            pop ds

                            lea si, password
                            mov di, sp

                            xor ch, ch
                            mov cx, [bp+4]
                            cmp cx, PASSWORD_LENGTH
                            jne @@end

                            repe cmpsb
                            jne @@end
                            inc dx

@@end:                      mov sp, bp
                            pop bp
                            ret 2
cmpPasswords                endp
```
Input file that hacks the program:

<img width="1032" height="492" alt="image" src="https://github.com/user-attachments/assets/3d14905d-cb0e-4490-88d1-0f7e1b01afef" />

The 02 and 03 bytes contain the address of the function "printfSuccessMessage" (little-endian). 0FFh byte - ASCII code of [Enter].

## Hacking a partner's program  
I received the file "krakra.com" from my colleague. I used IDA disassembler and here are my steps to crack his program:

#### 1. Quick look  

By looking through the program code in IDA's text mode, I noticed data buffer embedded within the code. This reminded me of my buffer overflow vulnerability. Then I started looking for a function that handles user input. This was the first function in the program. It does not check the length of the entered password and writes it to the address of this buffer.

<img width="1695" height="1054" alt="image" src="https://github.com/user-attachments/assets/66642605-1d66-4d96-9386-cebe94377684" />

To exploit this vulnerability, I created a file containing 53 junk bytes (the difference between 018ch (the address of the function following the buffer) and 0157h (the address of the buffer's beginning)). I followed these by the "EB" byte (the command "JMP" byte code), and the "94" byte (the difference between the address of the output "access approved" phrase and the current IP address). I ended the file with the "0D" byte (the [Enter] ASCII code).

<img width="1021" height="642" alt="image" src="https://github.com/user-attachments/assets/c49f9dd4-e5cd-49b5-8229-f96d2cee2fcf" />

Using input redirection I got a success message:

<img width="745" height="90" alt="image" src="https://github.com/user-attachments/assets/386af1fe-017c-4492-9f9b-17e64e1800c3" />










