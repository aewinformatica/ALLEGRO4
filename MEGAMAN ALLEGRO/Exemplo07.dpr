program Exemplo07;

uses
  Forms,
  Exemplo07Prj in 'Exemplo07Prj.pas' {Form1};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
