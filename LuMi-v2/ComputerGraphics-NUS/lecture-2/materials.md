Got it. From this demo, your **baseline stack + OpenGL “dialect”** for future assignments is:

## What version / pipeline this code is using

**OpenGL 3.3 Core Profile** (modern OpenGL).
You explicitly request it here:

```cpp
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
```

So for future work, you should stick to:

* **VAO/VBO required**
* **Shaders required** (no fixed pipeline)
* **glBegin/glEnd, glMatrixMode, glTranslatef/glRotatef, immediate mode** → **NOT allowed** (they’re removed in core profile)

## Libraries and their roles (as used here)

* **GLFW**: window/context creation + input callbacks (`glfwCreateWindow`, `glfwSetKeyCallback`, etc.)
* **GLEW**: loads OpenGL function pointers (`glewInit()`), needed on Windows for modern OpenGL calls
* **AntTweakBar**: simple UI overlay, integrated via GLFW callbacks + `TwDraw()`
* **GLM**: math library; you build transforms with `glm::translate/rotate` and pass a `mat4` uniform
* **ShaderProgram.h**: your wrapper that compiles/links shaders and sets uniforms (e.g., `setUniform("transform", sceneMat)`)

## Key “style rules” your future assignments should follow

### 1) Data goes to GPU via buffers

You create geometry like this:

* `glGenVertexArrays / glBindVertexArray`
* `glGenBuffers / glBindBuffer(GL_ARRAY_BUFFER)`
* `glBufferData(...)`
* `glVertexAttribPointer(...) + glEnableVertexAttribArray(...)`

So future models (even with Assimp) should end up as **VBOs/VAOs** with attribute layouts.

### 2) Attribute layout pattern

Your vertex format here is interleaved: **vec2 position + vec3 color** = 5 floats/vertex

```cpp
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));
```

So it strongly suggests your vertex shader uses something like:

* `layout(location=0) in vec2 aPos;`
* `layout(location=1) in vec3 aColor;`

If later you move to 3D, you’ll likely switch to **vec3 position**, plus normals/UVs, etc.—but still using **layout(location=...)** and VAOs.

### 3) Transform is a shader uniform (not fixed pipeline)

You do:

* build `glm::mat4`
* send to shader as uniform `"transform"`
* draw

So expect future assignments to keep using uniforms like `model/view/proj` (or `transform`) and multiply in the vertex shader.

### 4) Rendering approach

You’re drawing with `glDrawArrays`:

* ground, truck: `GL_TRIANGLES`
* wheels: `GL_TRIANGLE_FAN`

So even if you switch to indexed meshes later, the “modern” equivalent will be:

* `glDrawElements(...)` with an EBO, still core-profile compliant.

### 5) AntTweakBar integration contract

To keep AntTweakBar working you must:

* forward input callbacks (`TwEventMouseButtonGLFW`, `TwEventKeyGLFW`, etc.)
* call `TwDraw()` after your scene rendering
* call `TwInit(TW_OPENGL_CORE, NULL)` (note: core profile mode)

## Practical takeaway for your future assignments

When you write “the correct version” later, it should look like this demo structurally:

1. Create window/context (GLFW), init GLEW
2. Compile/link shaders (your `ShaderProgram`)
3. Create VAOs/VBOs (and maybe EBOs)
4. Render loop: clear → set polygon mode → `shader.use()` → set uniforms → bind VAO → draw → `TwDraw()` → swap/poll

If you paste your `window.vert` / `window.frag` and/or `ShaderProgram.h`, I can lock in the exact expected shader interface (uniform names, attribute locations, expected matrix order) so your later assignments match perfectly.



Here’s what Assignment 2 is asking for, what carries over from your Assignment 1 demo, and what you’ll need to add/change. 

## Assignment 2 requirements summary

### 1) Multiple viewports (5 marks)

You must render the same 3D scene in **multiple camera views** on one window: 

* **Top-down view** using **orthographic projection**
* **Front view** using **orthographic projection**
* **Perspective view** using **perspective projection**
* **Two lines** drawn to separate the views 
  (Page 1’s Figure 1 shows a 2×2 layout where three quadrants are views, and lines separate them.) 

### 2) 3D scene (10 marks)

Build a room scene: 

* **4 walls + floor** (no ceiling required)
* Lit by **one point light**
* **Floor textured**
* **Walls use normal mapping**
* Add objects:

  * **Painting on wall** using **textured polygons**
  * **Rotating ornament** using **cube environment mapping**; rotation by updating its transform matrix 

### 3) User interaction (5 marks)

Use a control panel UI to: 

* Show **FPS** and **frame time**
* Toggle **animation pause/unpause**
* Move **light position**
* Change **yaw/pitch** of the **perspective camera**

### 4) Submission / constraints

* Must use **modern shader-based OpenGL** and **only the libraries covered in the subject** 
* Include a **report with screenshots** showing features working 
* Quality matters; non-compiling/crashing caps marks heavily 

## What you can borrow directly from Assignment 1

### Rendering loop + timing

* Your `updateFPS()` logic and the displayed values map *exactly* to the “frame statistics” requirement (FPS + frame time). 
* The basic game loop structure (deltaTime, poll events, swap buffers) is reusable.

### AntTweakBar UI scaffolding

* The whole pattern of:

  * `TwInit(TW_OPENGL_CORE, NULL)`
  * forwarding GLFW callbacks to `TwEvent...`
  * creating a bar, `TwAddVarRO/RW`, `TwDraw()`
    is directly reusable for the required control panel. 
    You’ll just add new controls: animation toggle, light position (x/y/z), camera yaw/pitch.

### “Modern OpenGL” setup

* GLFW context hints for OpenGL **3.3 core** and shader-based pipeline match the “modern shader-based OpenGL” requirement. 
* VAO/VBO workflow (glGenVertexArrays, glVertexAttribPointer, glDrawArrays) is still the right style.

### Transform-uniform approach

* Your GLM matrix composition + sending a matrix uniform is the same approach you’ll use, just expanded to **model/view/projection** (and per-viewport camera/projection).

## What must change / be added for Assignment 2

### 1) From 2D to real 3D camera pipeline

Assignment 1 is basically 2D coordinates and a single `transform`.
Assignment 2 requires:

* 3D vertex positions (vec3)
* a camera **view matrix** (lookAt)
* a **projection matrix** (ortho or perspective)
* and per-object **model matrix**

In practice you’ll likely move to:
`MVP = projection * view * model` (or separate uniforms).

### 2) Multiple viewports

You must render the scene **3 times per frame**, changing:

* `glViewport(x, y, w, h)`
* projection type (ortho vs perspective)
* view matrix (top-down camera vs front camera vs yaw/pitch camera)

Also, you must “draw two lines to separate the views”  — simplest way is:

* render thin rectangles/lines in NDC using a tiny “UI line shader”, OR
* use `glScissor` + clear different regions, then draw separator geometry on top.

### 3) Lighting (point light)

You need actual shading:

* normals in your mesh data
* light position uniform
* fragment shader computing at least diffuse (and likely specular)

### 4) Texturing + normal mapping + cubemaps

You’ll need three material paths:

* **Floor texture** (diffuse texture sampling) 
* **Walls normal mapping** (tangent space normal map; needs tangents/bitangents or derived tangents + TBN matrix) 
* **Ornament environment mapping** using a cube map and reflective lookup based on view direction + normal; plus rotation animation 

### 5) User controls required

Add UI-controlled variables for: 

* `bool animate`
* `glm::vec3 lightPos`
* `float yaw, pitch` (only affects the **perspective viewport camera**)

## A clean way to structure Assignment 2 (based on your Assignment 1 style)

* Keep your existing:

  * window init, callbacks, FPS tracking, Tw setup
* Expand scene init into:

  * `initShaders()` (maybe separate shaders: textured/normalmapped/envmap/lines)
  * `initGeometry()` (room meshes + painting quad + ornament mesh)
  * `initTextures()` (floor diffuse, wall diffuse+normal, painting texture, cubemap)
* In render loop:

  * update animation if not paused
  * render each viewport:

    * set viewport
    * set projection + view
    * draw room + painting + ornament
  * draw separators
  * TwDraw

If you want, paste (or upload) your provided starter code for Assignment 2 (if any), and I’ll map **exactly** which parts of your Assignment 1 file can be copied verbatim, and which functions/classes you’ll need to add (camera, texture loader, cubemap loader, normal-mapped shader inputs, etc.).



I dug into `DemoCode.zip` (unzipped + read the source). It’s a **lab demo that teaches exactly the OpenGL texture “knobs”** you’ll need for Assignment 2’s **floor texture + painting texture + (indirectly) normal map + cubemap** requirements. 

> Note: I can’t attach line-citations for the ZIP’s `.cpp/.h/.glsl` the same way as the PDF (the ZIP isn’t indexed by the citation tool), but the details below come directly from the code inside the archive.

---

## What the demo actually demonstrates (and why it matters)

### 1) Texture loading pipeline you’ll reuse

The demo uses **stb_image** to load `./images/check.bmp`, uploads it with `glTexImage2D`, and builds mipmaps with `glGenerateMipmap`. This is the standard pattern you want for Assignment 2 textures:

* floor diffuse texture 
* painting texture 
* wall normal map (also a 2D texture) 

### 2) Texture **wrap modes** (tiling vs clamping)

The demo exposes Wrap **S** and **T** in AntTweakBar:

* `GL_REPEAT`
* `GL_MIRRORED_REPEAT`
* `GL_CLAMP_TO_EDGE`
* `GL_CLAMP_TO_BORDER` (and sets a red border color)

Why you care in Assignment 2:

* **Floor**: usually looks best with `GL_REPEAT` so the pattern tiles across the floor.
* **Painting**: often `GL_CLAMP_TO_EDGE` so you don’t get repeated edges if UVs slightly overshoot.
* **Normal map on walls**: typically also `GL_REPEAT` if you want a repeating wall pattern.

The demo intentionally uses texture coordinates going from **0 to 10** on a quad, so you *visually* see wrapping/tiling changes immediately.

### 3) Texture **filtering** (pixelated vs smooth vs mipmapped)

The demo exposes:

* MAG filter: `NEAREST` vs `LINEAR`
* MIN filter: `NEAREST`, `LINEAR`, plus the mipmap variants (`*_MIPMAP_*`)

Why you care in Assignment 2:

* Your floor/walls will be viewed at steep angles and varying distance. **Mipmap min filters** (commonly `GL_LINEAR_MIPMAP_LINEAR`) reduce shimmering and aliasing a lot.
* Painting viewed from a distance benefits from mipmaps too.

Practical “good defaults” for A2 (not the only choice, but solid):

* `GL_TEXTURE_MIN_FILTER = GL_LINEAR_MIPMAP_LINEAR`
* `GL_TEXTURE_MAG_FILTER = GL_LINEAR`
* plus `glGenerateMipmap(GL_TEXTURE_2D)` after uploading.

### 4) “Replace” vs “modulate” texturing

The demo has a UI toggle `Replace`:

* **Replace**: output color = texture color
* **Not replace**: output color = lighting * texture color

Why you care in Assignment 2:

* Floor/walls/painting are typically **lit** (so modulate is common).
* If you want the painting to look “self-lit”/flat (sometimes nicer), you can implement a similar “replace” option per-material.

### 5) It’s already in the same “modern OpenGL 3.3 core + AntTweakBar” style

This demo matches your Assignment 1 environment choices (GLFW + GLEW + AntTweakBar, OpenGL 3.3 core). That means you can borrow a lot of structure:

* GLFW callbacks forwarding to Tw
* shader wrapper usage (`ShaderProgram`)
* VAO/VBO layout with positions/normals/UVs
* `MVP = projection * view * model`

---

## How this connects to Assignment 2 features

Assignment 2 explicitly requires: floor textured, walls normal mapped, painting textured, ornament cubemap environment mapped. 

Here’s the mapping:

### Floor texture (directly covered by demo)

Use the demo’s 2D texture setup almost 1:1.

* Wrap: **REPEAT** (common for floors)
* Filters: **mipmapped min filter** + **linear mag filter**

### Painting texture (directly covered by demo)

Same 2D texture pipeline, but the geometry is a wall quad.

* Wrap: usually **CLAMP_TO_EDGE**
* Filters: mipmaps help when seen from afar

### Wall normal mapping (demo gives you the *texture parameter* side)

Normal mapping needs *more than* the demo (tangents/TBN + a different fragment shader), but:

* the **normal map is still just a `sampler2D`** with wrap/filter settings exactly like the demo
* you’ll likely load **two textures** for walls: diffuse + normal

### Ornament cubemap environment mapping (demo doesn’t implement cubemaps, but the idea transfers)

Cubemaps use different target + parameters:

* `glBindTexture(GL_TEXTURE_CUBE_MAP, ...)`
* Wrap must be `GL_CLAMP_TO_EDGE` on **S/T/R** to avoid seams
* Filtering commonly `GL_LINEAR` and mipmaps optional depending on how fancy you go

Rotation requirement: “rotate by updating its transformation matrix.” 
That part is like your Assignment 1 wheel rotation concept, just in 3D.

---

## What you should “borrow” from this demo into Assignment 2

If I were you, I’d lift these ideas/components:

1. A small **texture loading helper** (stb_image → glTexImage2D → glGenerateMipmap) to reuse for floor/walls/painting.
2. A **material/texture parameter block** per object (wrap/filter choices).
3. UI patterns for:

   * light position sliders (the demo already does this)
   * toggles (wireframe/replace) which is the same style you’ll use for **pause/unpause** in A2 
4. The vertex format pattern: **position + normal + texcoord** (you will need this in A2 for lighting + textures).

---

## Quick “next step” suggestion

If you tell me whether your Assignment 2 starter code already loads any textures (or if you’re starting from Assignment 1), I can outline a clean minimum path:

* **Step 1:** get floor + painting textures working (using demo pipeline)
* **Step 2:** add point light shading (demo already has point-light shader structure)
* **Step 3:** add multi-viewport cameras 
* **Step 4:** add normal mapping to walls 
* **Step 5:** add cubemap environment mapping ornament 

If you want, paste your current Assignment 2 “main.cpp” (or upload it) and I’ll point out exactly where the demo’s texture-parameter logic plugs in.
