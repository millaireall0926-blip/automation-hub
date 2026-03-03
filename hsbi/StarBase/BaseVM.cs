using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Windows.Input;

namespace StarBase
{
    public class BaseVM : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        public void OnPropertyChanged(string propertyName) => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }


    public class DelegateCommand : ICommand
    {
        private readonly Predicate<object> _canExecute;
        private readonly Action<object> _execute;
        private event EventHandler _canExecuteChangedInternal;

        public DelegateCommand() { }
        public DelegateCommand(Action<object> execute) : this(execute, null)
            => this._execute = execute;

        public DelegateCommand(Action<object> execute, Predicate<object> canExecute)
        {
            this._execute = execute ?? throw new ArgumentNullException("execute");
            this._canExecute = canExecute;
        }

        public bool CanExecute(object parameter) => _canExecute == null || _canExecute(parameter);

        public void Execute(object parameter) => this._execute(parameter);

        public event EventHandler CanExecuteChanged
        {
            add
            {
                CommandManager.RequerySuggested += value;
                _canExecuteChangedInternal += value;
            }
            remove
            {
                CommandManager.RequerySuggested -= value;
                _canExecuteChangedInternal -= value;
            }
        }

        public void RaiseCanExecuteChanged() => _canExecuteChangedInternal.Raise(this);

    }

    public class EventArgs<T> : EventArgs
    {
        public T Value { get; private set; }
        public EventArgs(T value) => this.Value = value;
    }

    public static class EventRaiser
    {
        public static void Raise(this EventHandler handler, object sender)
            => handler?.Invoke(sender, EventArgs.Empty);

        public static void Raise<T>(this EventHandler<EventArgs<T>> handler, object sender, T value)
         => handler?.Invoke(sender, new EventArgs<T>(value));

        public static void Raise<T>(this EventHandler<T> handler, object sender, T value) where T : EventArgs
         => handler?.Invoke(sender, value);

        public static void Raise<T>(this EventHandler<EventArgs<T>> handler, object sender, EventArgs<T> value)
         => handler?.Invoke(sender, value);
    }

}