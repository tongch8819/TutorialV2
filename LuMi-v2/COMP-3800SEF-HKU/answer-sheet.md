# Answer Key: Web Development Diagnostic Assessment

### Part A: Foundations (60%)
1. [cite_start]**Web Container**: It manages the lifecycle of servlets and maps URLs to specific components.
2. **Servlet**: A Java class that extends the capabilities of a server. [cite_start]Lifecycle: init(), service(), and destroy().
3. **JSP vs. Servlet**: JSP is document-centric (HTML with Java), while Servlets are code-centric. [cite_start]Use JSP for presentation.
4. [cite_start]**JavaBean**: A reusable software component (class) with a no-arg constructor and getter/setter methods used to encapsulate many objects into one.
5. [cite_start]**Cycle**: Client sends HTTP request -> Server/Container processes -> Servlet generates dynamic content -> HTTP response sent back.
6. [cite_start]**Data Sharing**: Through scopes like Request, Session, or Application context.
7. [cite_start]**Application Server**: Provides the environment (JVM, libraries, container) needed to execute Java EE specifications.

### Part B: Advanced Concepts (40%)
8. **MVC Models**: Model 1 uses JSP for everything; [cite_start]Model 2 (Recommended) uses Servlets as controllers to separate logic from view.
9. [cite_start]**Spring Boot**: It provides "Auto-configuration" and embedded servers, making it much faster to start a project compared to standard Spring MVC.
10. **DAO & JPA**: DAO hides the details of data storage. [cite_start]JPA/Hibernate automates the mapping of Java objects to database tables (ORM).
11. [cite_start]**Dependency Injection**: It is a pattern where an object's dependencies are provided by an external container rather than created by the object itself.
12. **Filters & Security**: Filters intercept requests before they reach the controller. [cite_start]Spring Security uses them for authentication and authorization checks.