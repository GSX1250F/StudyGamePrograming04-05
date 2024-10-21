Imports OpenTK.Mathematics

Public Class MoveComponent
    Inherits Component

    'public
    Sub New(ByRef owner As Actor, ByVal updateOrder As Integer)
        MyBase.New(owner, updateOrder)
        mVelocity = Vector3.Zero
        'mRotSpeed = 0.0
        mRotSpeed = Vector3.Zero
        mMass = 1.0
        mMoveForce = Vector3.Zero
        'mRotForce = 0.0
        mRotForce = Vector3.Zero
        mMoveResist = 0.0
        mRotResist = 0.0
    End Sub

    Public Overrides Sub Update(deltaTime As Double)
        If (mVelocity.Length > 0) Then
            mOwner.SetPosition(mOwner.GetPosition() + mVelocity * deltaTime)    ' Actorの位置を更新
        End If
        If (mRotSpeed.Length > 0) Then
            'mOwner.SetRotation(mOwner.GetRotation() + mRotSpeed * deltaTime)    ' Actorの方向を更新
            '向きを更新
            Dim rot As Quaternion = mOwner.GetRotation()
            'クォータニオン生成。回転速度モーメントベクトルの大きさが角速度の大きさ
            Dim axis As Vector3 = mRotSpeed
            axis.Normalize()    '回転軸。正規化する。
            Dim angle As Double = deltaTime * mRotSpeed.Length()    '角度変化量
            Dim inc = Quaternion.FromAxisAngle(axis, angle)
            'もとのrotと増分のクォータニオンを結合
            rot = Quaternion.Multiply(rot, inc)
            mOwner.SetRotation(rot)
        End If

        SetVelocity(mVelocity + GetMoveAccel() * deltaTime)     'v = v0 + at
        SetRotSpeed(mRotSpeed + GetRotAccel() * deltaTime)     'ω = ωo + bt
    End Sub
    Public Sub SetMass(ByVal v As Double)
        mMass = v
    End Sub
    Public Function GetImoment() As Double
        ' 慣性モーメント計算　※※2次元においては、一様密度の円板とする。 I=0.5*質量*半径^2
        Return 0.5 * mMass * mOwner.GetRadius() * mOwner.GetRadius()
    End Function
    Public Sub SetMoveResist(ByVal v As Double)
        mMoveResist = v
    End Sub
    Public Sub SetRotResist(ByVal v As Double)
        mRotResist = v
    End Sub

    Public Sub SetVelocity(ByVal v)
        mVelocity = v
    End Sub
    Public Function GetVelocity() As Vector3
        Return mVelocity
    End Function
    Public Sub SetRotSpeed(ByVal v As Vector3)
        'Public Sub SetRotSpeed(ByVal v As Double)
        mRotSpeed = v
    End Sub
    Public Function GetRotSpeed() As Vector3
        Return mRotSpeed
    End Function
    Public Sub SetMoveForce(ByVal v As Vector3)
        mMoveForce = v
    End Sub
    Public Sub SetRotForce(ByVal v As Vector3)
        'Public Sub SetRotForce(ByVal v As Double)
        mRotForce = v
    End Sub
    Public Function GetMoveAccel() As Vector3
        If mMass = 0 Then
            Return Vector3.Zero
        Else
            Dim accel As Vector3 = mMoveForce * (1 / mMass)    '重心加速度の計算　F=ma  a=F*(1/m)
            '抵抗力 = 速さ*抵抗係数    減速 = -速さ*抵抗係数/質量
            accel -= mVelocity * mMoveResist * 0.01 * (1 / mMass)
            Return accel
        End If
    End Function
    Public Function GetTorque() As Vector3
        'Public Function GetTorque() As Double
        'トルク計算　トルク=回転方向の力 * 半径
        Return mRotForce * mOwner.GetRadius()
    End Function
    Public Function GetRotAccel() As Vector3
        'Public Function GetRotAccel() As Double
        If GetImoment() = 0 Then
            'Return 0
            Return Vector3.Zero
        Else
            '回転加速度の計算　回転加速度 = トルク / 慣性モーメント
            Dim accel As Vector3 = GetTorque() / GetImoment()    '回転加速度の計算 Fr=Ia  a=Fr/I
            '抵抗力 = 速さ*抵抗係数    減速 = -速さ*抵抗係数*半径/慣性モーメント
            accel -= mRotSpeed * mOwner.GetRadius() * mRotResist / GetImoment()
            Return accel
        End If
    End Function

    'private
    Private mMass As Double      '質量
    Private mMoveResist As Double        '重心速度抵抗率(%)
    Private mRotResist As Double     '回転速度抵抗率(%)

    Private mVelocity As Vector3     '並進移動速度
    'Private mRotSpeed As Double      '回転速度
    Private mRotSpeed As Vector3      '回転速度
    Private mMoveForce As Vector3        '重心にかかる力
    'Private mRotForce As Double          '回転方向の力F　 トルク=RotForce * Radius = Imoment * RotAccel
    Private mRotForce As Vector3          '回転方向の力F　 トルク=RotForce * Radius = Imoment * RotAccel

End Class