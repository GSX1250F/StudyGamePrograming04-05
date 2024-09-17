Imports OpenTK

Public Class NavComponent
    Inherits MoveComponent
    Sub New(ByRef owner As Actor, ByVal updateOrder As Integer)
        MyBase.New(owner, updateOrder)
        mNextNode = Nothing
    End Sub
    Public Overrides Sub Update(deltaTime As Double)
        If (mNextNode IsNot Nothing) Then
            Dim diff As Vector3 = mNextNode.GetPosition() - mOwner.GetPosition()
            Dim nextspeed = speed
            If (speed * deltaTime > diff.Length) Then nextspeed = Int(diff.Length) / deltaTime
            If (diff.Length <= speed * deltaTime) Then
                mNextNode = mNextNode.GetParent()
            End If
            mNextDirVector = Vector3.Normalize(diff)
            mNextDirAngle = Math.Atan2(-mNextDirVector.Y, mNextDirVector.X)
            SetVelocity(nextspeed * mNextDirVector)
        End If
        MyBase.Update(deltaTime)
    End Sub
    Public Sub SetSpeed(ByVal value As Double)
        speed = value
    End Sub
    Public Sub SetStartNode(ByRef start As Tile)
        If (start IsNot Nothing) Then mNextNode = start.GetParent()
    End Sub
    Public Function GetNextNode() As Tile
        Return mNextNode
    End Function
    Public Function GetNextDirVector() As Vector3
        Return mNextDirVector
    End Function
    Public Function GetNextDirAngle() As Double
        Return mNextDirAngle
    End Function

    Private mNextNode As Tile
    Private speed As Double
    Private mNextDirVector As Vector3
    Private mNextDirAngle As Double
End Class
