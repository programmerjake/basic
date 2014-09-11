For Dim i = 3 To 10000 Step 2
    Dim isPrime As Boolean = True
    For Dim j = 3 To i - 1 Step 2
        If i Mod j = 0 Then
            isPrime = False
            j = i
        End If
    Next
    isPrime
Next