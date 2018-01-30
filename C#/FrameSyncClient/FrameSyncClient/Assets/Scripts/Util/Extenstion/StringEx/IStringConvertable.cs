using UnityEngine;
using System.Collections;

public interface IStringConvertable<T> {

    string ConvertToString();
    T GetValue(string value);

}
