Dim a As Int64 = 100
a = 23_64
Dim b As ByRef Int64 = a, c As ByRef Int64 = a
If b = 100000 Then
    b = CInt(45.4)
End If