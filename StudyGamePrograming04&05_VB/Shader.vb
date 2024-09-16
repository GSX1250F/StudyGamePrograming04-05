Imports System.IO
Imports System.Reflection.Metadata
Imports System.Runtime.CompilerServices
Imports OpenTK
Imports OpenTK.Graphics
Imports OpenTK.Graphics.OpenGL
Imports OpenTK.Mathematics
Public Class Shader
    Implements IDisposable      '明示的にクラスを開放するために必要
    'public
    Public Sub New()
        mShaderProgram = 0
        mVertexShader = 0
        mFragShader = 0
    End Sub
    Protected disposed = False     '開放処理が実施済みかのフラグ
    Public Overloads Sub Dispose() Implements IDisposable.Dispose
        Dispose(True)
    End Sub
    Protected Overridable Overloads Sub Dispose(ByVal disposing As Boolean)
        If Not Me.disposed Then
            If disposing Then
                '*** アンマネージリソースの開放
            End If
            '*** マネージドリソースの開放
        End If
        disposed = True
    End Sub
    Protected Overrides Sub Finalize()
        MyBase.Finalize()
        Dispose(False)
    End Sub
    Public Function Load(ByRef vertName As String, ByRef fragName As String)
        If CompileShader(vertName, ShaderType.VertexShader, mVertexShader) = False Then
            Return False
        End If
        If CompileShader(fragName, ShaderType.FragmentShader, mFragShader) = False Then
            Return False
        End If

        ' バーテックス,フラグメントシェーダーをリンクして
        ' シェーダープログラムを作る
        mShaderProgram = GL.CreateProgram()
        GL.AttachShader(mShaderProgram, mVertexShader)
        GL.AttachShader(mShaderProgram, mFragShader)
        GL.LinkProgram(mShaderProgram)

        ' プログラムが正しくリンクされたことを確認
        If (IsValidProgram() = False) Then
            Return False
        End If

        Return True
    End Function
    Public Sub Unload()
        GL.DeleteProgram(mShaderProgram)
        GL.DeleteShader(mVertexShader)
        GL.DeleteShader(mFragShader)
    End Sub
    Public Sub SetActive()
        GL.UseProgram(mShaderProgram)
    End Sub
    Public Sub SetMatrixUniform(ByRef name As String, ByRef matrix As Matrix4)
        'nameと同じuniform変数をシェーダープログラムから探し、そのIDを受け取る。
        Dim uniformId As Integer = GL.GetUniformLocation(mShaderProgram, name)
        'matrixで上書き
        GL.UniformMatrix4(uniformId, True, matrix)
    End Sub

    'private
    Private disposedValue As Boolean
    Private Function CompileShader(ByRef fileName As String, ByVal shaderType As ShaderType, ByRef outShader As Integer)
        If System.IO.File.Exists(fileName) Then
            'すべてのテキストを1つの文字列に読み込む
            Dim sstream = File.ReadAllText(fileName)

            ' 指定されたタイプのシェーダーを作成
            outShader = GL.CreateShader(shaderType)
            ' 読み込んだ文字列でのコンパイルを試みる
            GL.ShaderSource(outShader, sstream)
            GL.CompileShader(outShader)

            If (IsCompiled(outShader) <> True) Then
                Console.WriteLine("シェーダー %s のコンパイルに失敗しました", fileName)
                Return False
            End If
        Else
            Console.WriteLine("シェーダーファイル %s が見つかりません", fileName)
            Return False
        End If

        Return True
    End Function
    Private Function IsCompiled(ByVal shader As Integer) As Boolean
        Dim status As Integer
        ' コンパイル状態を問い合わせる
        GL.GetShader(shader, ShaderParameter.CompileStatus, status)
        If (status = 0) Then
            Dim infoLog As String = GL.GetShaderInfoLog(shader)
            Console.WriteLine("GLSLのコンパイルが失敗しました：\n%s", infoLog)
            Return False
        End If

        Return True
    End Function
    Private Function IsValidProgram() As Boolean
        Dim status As Integer
        ' コンパイル状態を問い合わせる
        GL.GetProgram(mShaderProgram, GetProgramParameterName.LinkStatus, status)
        If (status = 0) Then
            Dim infoLog As String = GL.GetProgramInfoLog(mShaderProgram)
            Console.WriteLine("GLSL Link Status:\n%s", infoLog)
            Return False
        End If

        Return True
    End Function
    Private mVertexShader As Integer
    Private mFragShader As Integer
    Private mShaderProgram As Integer
End Class
