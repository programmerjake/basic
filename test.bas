Declare Type T

Declare Sub test(As ByRef T)

Type T
    A As Integer
    B As Single
End Type

Dim V As T
V.B = V.A
test(V)

Sub test(a As ByRef T)
    a.a = CInt(a.b)
End Sub