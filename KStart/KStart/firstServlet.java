import java.io.PrintStream;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

// @WebServlet (name="firstServlet", urlPatterns={"/firstServlet"})

public class firstServlet extends HttpServlet{

	//客户端的响应方法
	public void service(HttpServletRequest request,HttpServletResponse response)
		throws ServletException,java.io.IOException{

			request.setCharacterEncoding("GBK");
			response.setContentType("text/html;charSet=GBK");

			PrintStream out =new PrintStream(response.getOutputStream());

			out.println("<html>");
			out.println("<head>");
			out.println("<title>Servlet 测试</title>");
			out.println("</head>");
			out.println("<body>");
			out.println("It's Work!");
			out.println("</body>");
			out.println("</html>");		
	}
}