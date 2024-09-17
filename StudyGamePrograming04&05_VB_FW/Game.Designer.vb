<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Game
    Inherits System.Windows.Forms.Form

    'フォームがコンポーネントの一覧をクリーンアップするために dispose をオーバーライドします。
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Windows フォーム デザイナーで必要です。
    Private components As System.ComponentModel.IContainer

    'メモ: 以下のプロシージャは Windows フォーム デザイナーで必要です。
    'Windows フォーム デザイナーを使用して変更できます。  
    'コード エディターを使って変更しないでください。
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.glControl = New OpenTK.GLControl()
        Me.RunLoop = New System.Windows.Forms.Timer(Me.components)
        Me.SuspendLayout()
        '
        'glControl
        '
        Me.glControl.BackColor = System.Drawing.Color.Black
        Me.glControl.Location = New System.Drawing.Point(46, 32)
        Me.glControl.Name = "glControl"
        Me.glControl.Size = New System.Drawing.Size(509, 320)
        Me.glControl.TabIndex = 0
        Me.glControl.VSync = False
        '
        'Game
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(800, 450)
        Me.Controls.Add(Me.glControl)
        Me.Name = "Game"
        Me.Text = "StudyGamePrograming VB(Framework)"
        Me.ResumeLayout(False)

    End Sub

    Friend WithEvents glControl As OpenTK.GLControl
    Friend WithEvents RunLoop As Timer
End Class
