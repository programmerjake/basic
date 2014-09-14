Sub print(s As String)
End Sub

Function factorial(v As Integer) As Double
    If v <= 1 Then
        factorial = 1
    Else
        factorial = v * factorial(v - 1)
    End If
End Function

Function pow(a As Double, b As Integer) As Double
    If b < 0 Then
        pow = 1 / pow(a, b)
    Else
        Dim RetVal As Double = 1
        If b Mod 2 = 1 Then
            RetVal = a
        End If
        b = b \ 2
        Do While b > 0
            a = a * a
            If b Mod 2 = 1 Then
                RetVal = RetVal * a
            End If
            b = b \ 2
        Loop
        pow = RetVal
    End If
End Function

Function mySin(a As Double) As Double
    Dim RetVal As Double = 0
    For Dim i = 0 To 6
        RetVal = RetVal + (1 - (i Mod 2) * 2) * pow(a, i * 2 + 1) / factorial(i * 2 + 1)
    Next
    mySin = RetVal
End Function

Function isPrime(v As Integer) As Boolean
    Dim limit As Integer = CInt(Sqr(v))
    If v = 2 Or v = 3 Then
        isPrime = True
        Exit Function
    ElseIf v < 2 Then
        isPrime = False
        Exit Function
    ElseIf v Mod 2 = 0 Then
        isPrime = False
        Exit Function
    End If
    For Dim i = 3 To limit Step 2
        If v Mod i = 0 Then
            isPrime = False
            Exit Function
        End If
    Next
    isPrime = True
    Exit Function
End Function

For Dim i = 0 To 21
    Dim a = i * 4 * ATn(1) / 20
    print("Sin(" + Str(i) + " * pi / 20 = " + Str(a) + ") = " + Str(Sin(a)) + " : " + Str(mySin(a)) + Chr(10))
    If isPrime(i) Then
        print(Str(i) + " is prime." + Chr(10))
    Else
        print(Str(i) + " is not prime." + Chr(10))
    End If
    Do
        If i > 20 Then
            Exit For
        End If
    Loop Until True
Next