Declare Sub WriteByte As "putchar"(v As Integer)

Sub WriteChar(Ch As Integer)
    Ch = Ch Mod &H110000
    If Ch < &H80 Then
        WriteByte(Ch)
    ElseIf Ch < &H800 Then
        WriteByte(Ch \ &H40 + &HC0)
        WriteByte((Ch And &H3F) + &H80)
    ElseIf Ch < &H10000 Then
        WriteByte(Ch \ &H1000 + &HE0)
        WriteByte((Ch \ &H40 And &H3F) + &H80)
        WriteByte((Ch And &H3F) + &H80)
    Else
        WriteByte(Ch \ &H40000 + &HF0)
        WriteByte((Ch \ &H1000 And &H3F) + &H80)
        WriteByte((Ch \ &H40 And &H3F) + &H80)
        WriteByte((Ch And &H3F) + &H80)
    End If
End Sub

Sub WriteString(S As String)
    For Dim i = 1 To Len(S)
        WriteChar(Asc(Mid(S, i, 1)))
    Next
End Sub

Sub WriteLn()
    WriteChar(10)
End Sub

Sub WriteInteger(v As Integer)
    WriteString(Str(v))
End Sub

Sub WriteDouble(v As Double)
    WriteString(Str(v))
End Sub

Dim text As Array(160, 100) Of Integer
Dim picture As Array(160, 100) Of Single

Sub WriteAllText()
    For Dim y = LBound(text, 2) To UBound(text, 2)
        For Dim x = LBound(text, 1) To UBound(text, 1)
            WriteChar(text(x, y))
        Next
        If y < UBound(text, 2) Then
            WriteLn()
        End If
    Next
End Sub

Function getDitherValue(x As Integer, y As Integer) As Single
    x = CInt(CU64(x) Mod 16_u64)
    y = CInt(CU64(y) Mod 16_u64)
    x = x Xor y
    Dim v As Integer = 0
    If (x And &H1) <> 0 Then
        v = v Or &H80
    End If
    If (x And &H2) <> 0 Then
        v = v Or &H20
    End If
    If (x And &H4) <> 0 Then
        v = v Or &H8
    End If
    If (x And &H8) <> 0 Then
        v = v Or &H2
    End If
    If (y And &H1) <> 0 Then
        v = v Or &H40
    End If
    If (y And &H2) <> 0 Then
        v = v Or &H10
    End If
    If (y And &H4) <> 0 Then
        v = v Or &H4
    End If
    If (y And &H8) <> 0 Then
        v = v Or &H1
    End If
    getDitherValue = CSng(v + 1) / &H100
End Function

Sub DitherPicture()
    For Dim y = LBound(text, 2) To UBound(text, 2)
        For Dim x = LBound(text, 1) To UBound(text, 1)
            Dim v As Single = picture(x, y) * 3, ditherValue As Single = getDitherValue(x, y)
            If v < ditherValue Then
                text(x, y) = Asc("█")
            ElseIf v < ditherValue + 1 Then
                text(x, y) = Asc("▓")
            ElseIf v < ditherValue + 2 Then
                text(x, y) = Asc("▒")
            ElseIf v < ditherValue + 3 Then
                text(x, y) = Asc("░")
            Else
                text(x, y) = Asc(" ")
            End If
        Next
    Next
End Sub

Dim IterationCount = 256

Dim LightX As Single = 1 / Sqr(3!), LightY As Single = 1 / Sqr(3!), LightZ As Single = 1 / Sqr(3!)

Function ComputePixel(x As Single, y As Single) As Single
    Dim rSquared As Single = x * x + y * y
    If rSquared >= 1 Then
        ComputePixel = 0
        Exit Function
    End If
    Dim z As Single = Sqr(1 - rSquared)
    Dim dot As Single = LightX * x + LightY * y + LightZ * z
    If dot < 0 Then
        dot = 0
    End If
    ComputePixel = 0.35! + 0.65! * dot
End Function

Sub ComputePicture()
    For Dim y = LBound(text, 2) To UBound(text, 2)
        For Dim x = LBound(text, 1) To UBound(text, 1)
            Dim xf As Single = CSng(x - LBound(text, 1)) / (UBound(text, 1) - LBound(text, 1)) * 2 - 1
            Dim yf As Single = 1 - CSng(y - LBound(text, 2)) / (UBound(text, 2) - LBound(text, 2)) * 2
            picture(x, y) = ComputePixel(xf, yf)
        Next
    Next
End Sub

ComputePicture()
DitherPicture()
WriteAllText()
