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

For Dim i = 0 To 20
    Dim a = i * 4 * ATn(1) / 20
    print("Sin(" + Str(i) + " * pi / 20 = " + Str(a) + ") = " + Str(Sin(a)) + " : " + Str(mySin(a)) + Chr(10))
Next