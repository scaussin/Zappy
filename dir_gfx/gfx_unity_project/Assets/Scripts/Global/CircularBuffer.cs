using System;
using System.Text;
using UnityEngine;

public class CircularBuffer
{
	private static int		buff_size = 12096;
	private byte[]			buffer = new byte[buff_size];
	private int				len;
	private int				start;

	public CircularBuffer()
	{
	//	buff_size = GameManager.instance.ConnectionManager.buff_size;
	}

	/// <summary>
	/// Gets the first message from the circular buffer. Moves the head of reading.
	/// </summary>
	/// <returns>The first message from the buffer finishing by a line return.</returns>
	public string			getMsg()
	{
		string		buffStr;
		int			lenCmd;
		int			found;

		if ((buffStr = getBufferStr ()) != null)
		{
			found = buffStr.IndexOf ('\n');
			if (found > 0)
			{
				buffStr = buffStr.Substring (0, found);
				lenCmd = buffStr.Length + 1;
				erase (lenCmd);
			}
			else
			{
				return (null);
			}
		}
		else
		{
			return (null);
		}
		return (buffStr);
	}

	/// <summary>
	/// Extracts unsent/unread bytes from the buffer. also make the reading head adavances.
	/// Useful for Socket.send(). Not recommended for simple buffer reading;
	/// </summary>
	/// <returns>The buffer bytes.</returns>
	public byte[]			ExtractBufferBytes()
	{
		byte[]		retBytes = new byte[len + 1];
		int			i;

		if (len > 0)
		{
			i = 0;
			while (i < len)
			{
				retBytes[i] = buffer[(start + i) % buff_size];
				i++;
			}
		}
		else
		{
			return (null);
		}
		erase (len);
		return (retBytes);
	}

	/// <summary>
	/// Gets the buffer as string. Does NOT make the reading head advances.
	/// Useful for reading into the buffer.
	/// </summary>
	/// <returns>The buffer string.</returns>
	public string			getBufferStr()
	{
		char[]		retBuff = new char[len + 1];
		int			i;

		if (len > 0)
		{
			i = 0;
			while (i < len)
			{
				retBuff[i] = Convert.ToChar (buffer [(start + i) % buff_size]);
				i++;
			}
			retBuff [i] = '\0';
		}
		else
		{
			return (null);
		}
		return (new string(retBuff));
	}

	/// <summary>
	/// Erase the specified sent_len.
	/// </summary>
	/// <param name="sent_len">Sent length.</param>
	public void				erase(int sent_len)
	{
		start = (start + sent_len) % buff_size;
		if (sent_len <= len)
		{
			len -= sent_len;
		}
		else
		{
			len = 0;
		}
	}

	/// <summary>
	/// For the buffer's current length.
	/// If the length is more than zero, it means there are unprocessed modifications.
	/// </summary>
	/// <returns>The current length of the datas on the buffer.</returns>
	public int						getLen()
	{
		return (len);
	}

	/// <summary>
	/// Resets the buffer. Called by managers to impeach any waiting msg sending to the server.
	/// </summary>
	public void						ResetBuffer()
	{
		len = 0;
	}

	public void				pushMsg(string msg)
	{
		byte[] bytes_to_push;

		bytes_to_push = Encoding.ASCII.GetBytes(msg);
		pushBytes(bytes_to_push, msg.Length);
	}

	/// <summary>
	/// Pushs the bytes onto the buffer.
	/// </summary>
	/// <param name="to_write">To write.</param>
	/// <param name="size">Size.</param>
	public void				pushBytes(byte[] to_write, int size)
	{
		int		i;

		if (size > 0)
		{
			i = 0;
			if (len + size > buff_size)
			{
				Debug.LogWarning ("Buffer full");
				return ;
			}
			while (i < size)
			{
				buffer[(start + len + i) % buff_size] = to_write[i];
				i++;
			}
			len += i;
//			Debug.Log (System.Text.Encoding.UTF8.GetString (buffer));
		}
	}
}
