using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StarIDE.Controls
{

    public class ProcessEventArgs : EventArgs
    {
        public ProcessEventArgs() { }
        public ProcessEventArgs(string content) { Content = content; }      
        public ProcessEventArgs(int code) { Code = code; }
        public ProcessEventArgs(string content, int code) { Content = content; Code = code;}
        public string Content { get; private set; }
        public int? Code { get; private set; }
    }

    public delegate void ProcessEventHandler(object sender, ProcessEventArgs args);

    public class CmdControl : IDisposable
    {        
        private Process process;
        bool ExitFlag = false;

        public Process Process { get => process; }

        private StreamWriter inputWriter;
        private TextReader outputReader;
        private TextReader errorReader;
        private BackgroundWorker outputWorker = null;
        private BackgroundWorker errorWorker = null;
        
        public event ProcessEventHandler OnProcessOutput;
        public event ProcessEventHandler OnProcessError;
        public event ProcessEventHandler OnProcessExit;

        public bool IsProcessRunning 
        {
            get {
                try { return (process != null && process.HasExited == false); }
                catch { return false;}
            }
        }

        #region User Interface
        public CmdControl()
        {
            
        }

        public void createBackgroundWorker()
        {
            outputWorker = new BackgroundWorker();
            outputWorker.WorkerSupportsCancellation = true;
            outputWorker.WorkerReportsProgress = true;
            outputWorker.WorkerSupportsCancellation = true;
            outputWorker.DoWork += outputWorker_DoWork;
            outputWorker.ProgressChanged += outputWorker_ProgressChanged;

            errorWorker = new BackgroundWorker();
            errorWorker.WorkerSupportsCancellation = true;
            errorWorker.WorkerReportsProgress = true;
            errorWorker.WorkerSupportsCancellation = true;
            errorWorker.DoWork += errorWorker_DoWork;
            errorWorker.ProgressChanged += errorWorker_ProgressChanged;
        }

        public void StartProcess(string fileName, string arguments)
        {
            ExitFlag = false;
            
            //  Create the process start info.
            var processStartInfo = new ProcessStartInfo(fileName, arguments);
            StartProcess(processStartInfo);
        }

        public void StartProcess(ProcessStartInfo processStartInfo)
        {
            ExitFlag = false;
            //  Set the options.
            processStartInfo.UseShellExecute = false;
            processStartInfo.ErrorDialog = false;
            processStartInfo.CreateNoWindow = true;

            //  Specify redirection.
            processStartInfo.RedirectStandardError = true;
            processStartInfo.RedirectStandardInput = true;
            processStartInfo.RedirectStandardOutput = true;

            //  Create the process.
            process = new Process();
            process.EnableRaisingEvents = true;
            process.StartInfo = processStartInfo;
            process.Exited += currentProcess_Exited;

            //  Start the process.
            try
            {
                process.Start();
            }
            catch (Exception e)
            {
                //  Trace the exception.
                Trace.WriteLine("Failed to start process " + processStartInfo.FileName + " with arguments '" + processStartInfo.Arguments + "'");
                Trace.WriteLine(e.ToString());
                return;
            }

            //  Create the readers and writers.
            inputWriter = process.StandardInput;
            outputReader = TextReader.Synchronized(process.StandardOutput);
            errorReader = TextReader.Synchronized(process.StandardError);

            if (outputWorker == null)
                createBackgroundWorker();
            //  Run the workers that read output and error.            
            if (outputWorker.IsBusy) { 
                
                Task.Delay(5000); }
            outputWorker.RunWorkerAsync();
            errorWorker.RunWorkerAsync();

            
        }


        public void WriteInput(string input)
        {
            if (IsProcessRunning)
            {
                inputWriter.WriteLine(input);
                inputWriter.Flush();
            }
        }

        public void StopProcess()
        {            
            if (IsProcessRunning == false)
                return;
            ExitFlag = true;
            process.Kill();
        }
        #endregion

        #region Worker Delagate Function
        void outputWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            //  We must be passed a string in the user state.
            if (e.UserState is string)
            {
                //  Fire the output event.
                FireProcessOutputEvent(e.UserState as string);
            }
        }

        void outputWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            while (
                outputWorker != null &&
                outputWorker.CancellationPending == false)
            {
                if (outputReader == null) break;
                if (ExitFlag) break;
                //  Any lines to read?
                int count;
                var buffer = new char[1024];
                do
                {
                    var builder = new StringBuilder();
                    if (outputReader == null) break;                    
                    count = outputReader.Read(buffer, 0, 1024);
                    if (outputWorker == null) break;
                    builder.Append(buffer, 0, count);
                    outputWorker.ReportProgress(0, builder.ToString());
                } while (count > 0);

                System.Threading.Thread.Sleep(200);
            }
            
            
        }

        void errorWorker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            //  The userstate must be a string.
            if (e.UserState is string)
            {
                //  Fire the error event.
                FireProcessErrorEvent(e.UserState as string);
            }
        }

        void errorWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            while (
                errorWorker != null &&
                errorWorker.CancellationPending == false)
            {
                if (errorReader == null) break;
                if (ExitFlag) break;
                int count;
                var buffer = new char[1024];
                do
                {
                    var builder = new StringBuilder();
                    count = errorReader.Read(buffer, 0, 1024);
                    if (errorWorker == null) break;
                    builder.Append(buffer, 0, count);
                    errorWorker.ReportProgress(0, builder.ToString());
                } while (count > 0);

                System.Threading.Thread.Sleep(200);
            }
            
        }

        void currentProcess_Exited(object sender, EventArgs e)
        {
            //  Fire process exited.
            FireProcessExitEvent(process.ExitCode);

            //  Disable the threads.
            outputWorker.CancelAsync();
            errorWorker.CancelAsync();

            outputWorker.Dispose();
            errorWorker.Dispose();

            inputWriter.Dispose();
            outputReader.Dispose();
            errorReader.Dispose();  

            outputWorker = null;
            errorWorker = null;

            inputWriter = null;
            outputReader = null;
            errorReader = null;
            process = null;            
        }
        #endregion

        #region Event Occurs
        private void FireProcessOutputEvent(string content)
        {
            //  Get the event and fire it.
            var theEvent = OnProcessOutput;
            if (theEvent != null)
                theEvent(this, new ProcessEventArgs(content));
        }

        private void FireProcessErrorEvent(string content)
        {
            //  Get the event and fire it.
            var theEvent = OnProcessError;
            if (theEvent != null)
                theEvent(this, new ProcessEventArgs(content));
        }

        private void FireProcessExitEvent(int code)
        {
            //  Get the event and fire it.
            var theEvent = OnProcessExit;
            if (theEvent != null)
                theEvent(this, new ProcessEventArgs(code));
        }

        #endregion

        #region Close Action
        ~CmdControl() { Dispose(true); }

        protected virtual void Dispose(bool disposing)
        {
            if (outputWorker != null)
            {
                outputWorker.Dispose();
                outputWorker = null;
            }
            if (errorWorker != null)
            {
                errorWorker.Dispose();
                errorWorker = null;
            }
            if (process != null)
            {
                process.Dispose();
                process = null;
            }
            if (inputWriter != null)
            {
//                inputWriter.Dispose();
                inputWriter = null;
            }
            if (outputReader != null)
            {
                outputReader.Dispose();
                outputReader = null;
            }
            if (errorReader != null)
            {
                errorReader.Dispose();
                errorReader = null;
            }
        }

        

        public void Dispose()
        {        
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }

        #endregion
    }
}
